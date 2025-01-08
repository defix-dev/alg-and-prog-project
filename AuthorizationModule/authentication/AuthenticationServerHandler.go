package authentication

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
	"time"

	"authorization-module/authentication/common"
	"authorization-module/authentication/github"
	jwt_token "authorization-module/authentication/jwt"
	"authorization-module/authentication/yandex"
	"authorization-module/config"
	mongodb "authorization-module/database"
)

type AuthTokenData struct {
	ExpiresAt      int64
	ResponseStatus int
	AccessToken    string
	RefreshToken   string
}

var tokens = make(map[string]AuthTokenData)

const (
	GITHUB_AUTH_TYPE = "github"
	YANDEX_AUTH_TYPE = "yandex"
	CODE_AUTH_TYPE   = "code"
)

func StartHandle() {
	http.HandleFunc("/login", handleLoginPath)
	http.HandleFunc("/callback", handleCallbackPathAuth)
	http.HandleFunc("/authorize", handleAuthorizeAuth)
	http.HandleFunc("/login_end_stage", handleLoginEndStagePath)
	http.HandleFunc("/logout", handleLogoutPath)
}

func handleLogoutPath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()
	refreshToken := query.Get("refresh_token")
	if refreshToken == "" {
		sendErrorWithStatusCode(response, request, http.StatusBadRequest)
		return
	}

	token, err := jwt_token.DecryptRefreshToken(refreshToken)
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	mongodb.ConstructUserModificator().ModifyTokensByEmail(mongodb.TokenDetails{}, token.Email)
}

func handleLoginEndStagePath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()
	status := query.Get("status")
	code := query.Get("code")

	session, err := request.Cookie("JSESSIONID")
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	response.Header().Add("Content-Type", "text/html")
	response.Header().Add("Cookie", "JSESSIONID="+session.Value)

	if status == "success" {
		response.Write([]byte("<meta charset='UTF-8'/><h1>Успешно</h1><a href='http://localhost:3030/'>Вернуться на сайт</a>"))
	} else {
		errorMsg := "Во время авторизации возникла ошибка"
		if code != "" {
			errorMsg += ": " + code
		}
		response.Write([]byte("<meta charset='UTF-8'/><h1>" + errorMsg + "</h1><a href='http://localhost:3030/'>Вернуться на сайт</a>"))
	}
}

func handleLoginPath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()
	authType := query.Get("type")
	token := query.Get("token")

	tokens[token] = AuthTokenData{
		ExpiresAt:      time.Now().Add(5 * time.Minute).Unix(),
		ResponseStatus: http.StatusNotImplemented,
	}

	redirectURL := github.CreateStageOneRef(token)
	if authType == YANDEX_AUTH_TYPE {
		redirectURL = yandex.CreateStageOneRef(token)
	}

	http.Redirect(response, request, redirectURL, http.StatusFound)
}

func handleCallbackPathAuth(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()
	state := query.Get("state")

	if query.Has("error") || tokens[state].ExpiresAt < time.Now().Unix() {
		sendErrorWithStatusCode(response, request, http.StatusUnauthorized)
		return
	}

	code := query.Get("code")
	authType := query.Get("auth_type")

	var oauthKey string
	if authType == YANDEX_AUTH_TYPE {
		oauthKey = yandex.GetAccessToken(code)
	} else {
		oauthKey = github.GetAccessToken(code, state)
	}

	if oauthKey == "" {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	redirectURL := fmt.Sprintf("http://%s/authorize?access_token=%s&auth_type=%s&state=%s",
		config.MAIN_DOMAIN, oauthKey, authType, state)
	http.Redirect(response, request, redirectURL, http.StatusFound)
}

func handleAuthorizeAuth(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()

	accessToken := query.Get("access_token")
	authType := query.Get("auth_type")
	state := query.Get("state")

	if accessToken == "" || authType == "" {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	var jsonBody string
	if authType == YANDEX_AUTH_TYPE {
		jsonBody = yandex.ChangeCodeToUserInfo(accessToken)
	} else {
		jsonBody = github.ChangeCodeToUserInfo(accessToken)
	}

	if jsonBody == "" {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	var requiredData common.UserDataRequired
	var err error
	if authType == YANDEX_AUTH_TYPE {
		var dr *yandex.YandexUserDataRequired
		dr, err = yandex.DeserializeToRequiredData(jsonBody)
		requiredData = common.UserDataRequired{Email: dr.Email}
	} else {
		var dr *github.GithubUserDataRequired
		dr, err = github.DeserializeToRequiredData(jsonBody)
		requiredData = common.UserDataRequired{Email: dr.Email}
	}

	if err != nil || requiredData.Email == "" {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	finder := mongodb.ConstructUserFinder()
	user, err := finder.FindByEmail(requiredData.Email)
	register := false
	if err != nil {
		mongodb.ConstructUserCreator().CreateByEmail(requiredData.Email)
		user, _ = finder.FindByEmail(requiredData.Email)
		register = true
	}

	if user == nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	permissions, err := mongodb.GeneratePermissions(user.Role)
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	newAccessToken, _ := jwt_token.GenerateAccessToken(user.UserId, permissions)
	refreshToken, err := jwt_token.GenerateRefreshToken(user.Email)
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	if register {
		req, err := http.NewRequest("POST", "http://"+config.APP_DOMAIN+"/api/users/register?email="+user.Email, nil)
		if err != nil {
			sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
			return
		}
		req.Header.Add("Authorization", "Bearer "+newAccessToken)
		(&http.Client{}).Do(req)
	}

	if tokens[state].ExpiresAt < time.Now().Unix() {
		sendErrorWithStatusCode(response, request, http.StatusRequestTimeout)
		return
	}

	tokens[state] = AuthTokenData{
		ExpiresAt:      tokens[state].ExpiresAt,
		ResponseStatus: http.StatusOK,
		AccessToken:    newAccessToken,
		RefreshToken:   refreshToken,
	}

	mongodb.ConstructUserModificator().ModifyTokensByEmail(mongodb.TokenDetails{
		RefreshToken: refreshToken,
	}, user.Email)

	session, err := request.Cookie("JSESSIONID")
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	jsonData, err := json.Marshal(tokens[state])
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	req, err := http.NewRequest("POST", "http://"+config.WEB_DOMAIN+"/", bytes.NewReader(jsonData))
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}
	req.Header.Add("Content-Type", "application/json")
	req.AddCookie(session)

	_, err = (&http.Client{}).Do(req)
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	http.Redirect(response, request, "http://"+config.MAIN_DOMAIN+"/login_end_stage?status=success", http.StatusFound)
}
