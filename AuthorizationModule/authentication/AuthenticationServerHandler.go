package authentication

import (
	"authorization-module/authentication/github"
	jwt_token "authorization-module/authentication/jwt"
	"authorization-module/authentication/yandex"
	mongodb "authorization-module/database"
	"bytes"
	"encoding/json"
	"net/http"
	"strconv"
	"time"
)

type AuthTokenData struct {
	ExpiresAt      time.Time
	ResponseStatus int
}

type YandexUserDataRequired struct {
	Email string `json:"default_email"`
}

type GithubUserDataRequired struct {
	Email string `json:"email"`
}

var tokens map[string]AuthTokenData = make(map[string]AuthTokenData)

const (
	GITHUB_AUTH_TYPE = "github"
	YANDEX_AUTH_TYPE = "yandex"
	CODE_AUTH_TYPE   = "code"
	TEMP_LOGIN_TOKEN = "token"
)

func StartHandle() {
	http.HandleFunc("/login", handleLoginPath)
	http.HandleFunc("/callback", handleCallbackPath)
	http.HandleFunc("/authorize", handleAuthorize)
	http.HandleFunc("/login_end_stage", handleLoginEndStagePath)
}

func handleLoginEndStagePath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()
	status := query.Get("status")
	code := query.Get("code")

	response.Header().Add("Content-Type", "text/html")

	if status == "success" {
		response.Write([]byte("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><title>Success Page</title></head><body><h1>Успешно</h1><a href='http://localhost:3030/'>Вернуться на сайт</a></body></html>"))
	} else if code != "" {
		response.Write([]byte("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><title>Failed Page</title></head><body><h1>Во время авторизации возникла ошибка: " + code + "</h1><a href='http://localhost:3030/'>Вернуться на сайт</a></body></html>"))
	} else {
		response.Write([]byte("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><title>Failed Page</title></head><body><h1>Во время авторизации возникла ошибка</h1><a href='http://localhost:3030/'>Вернуться на сайт</a></body></html>"))
	}
}

func handleLoginPath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()
	authType := query.Get("type")
	token := query.Get(TEMP_LOGIN_TOKEN)

	tokens[token] = AuthTokenData{
		ExpiresAt:      time.Now().Add(5 * time.Minute),
		ResponseStatus: http.StatusNotModified,
	}

	if authType == YANDEX_AUTH_TYPE {
		http.Redirect(response, request, yandex.CreateStageOneRef(token), http.StatusFound)
	} else {
		http.Redirect(response, request, github.CreateStageOneRef(token), http.StatusFound)
	}
}

func handleCallbackPath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()

	state := query.Get("state")
	_, exists := tokens[state]

	if query.Has("error") || !exists {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	code := query.Get("code")
	authType := query.Get("auth_type")

	var oauthkey string
	if authType == YANDEX_AUTH_TYPE {
		oauthkey = yandex.GetAccessToken(code)
	} else {
		oauthkey = github.GetAccessToken(code, state)
	}

	http.Redirect(response, request, "http://localhost:3031/authorize?access_token="+oauthkey+"&auth_type="+authType, http.StatusFound)
}

func handleAuthorize(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()

	accessToken := query.Get("access_token")
	authType := query.Get("auth_type")

	if accessToken == "" || authType == "" {
		sendErrorWithStatusCode(response, request, http.StatusBadRequest)
		return
	}

	var jsonBody string
	if authType == "yandex" {
		jsonBody = yandex.ChangeCodeToUserInfo(accessToken)
	} else {
		jsonBody = github.ChangeCodeToUserInfo(accessToken)
	}

	if jsonBody == "" {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	var requiredData YandexUserDataRequired
	if err := json.Unmarshal([]byte(jsonBody), &requiredData); err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	if requiredData.Email == "" {
		http.Redirect(response, request, "http://localhost:3030/login?error="+strconv.Itoa(http.StatusBadRequest), http.StatusFound)
		return
	}

	finder := mongodb.ConstructUserFinder()
	user, err := finder.FindByEmail(requiredData.Email)
	if err != nil {
		mongodb.ConstructUserCreator().CreateByEmail(requiredData.Email)
		user, _ = finder.FindByEmail(requiredData.Email)
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

	accessToken, err = jwt_token.GenerateAccessToken(permissions)
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	refreshToken, err := jwt_token.GenerateRefreshToken(user.Email)
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	err = mongodb.ConstructUserModificator().ModifyTokensByEmail(mongodb.TokenDetails{
		RefreshToken: refreshToken,
		AuthToken:    "",
	}, user.Email)
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}

	_, err = http.Post("http://localhost:3030?access_token="+accessToken+"&refresh_token="+refreshToken, "application/x-www-form-urlencoded", bytes.NewBuffer([]byte{}))
	if err != nil {
		sendErrorWithStatusCode(response, request, http.StatusInternalServerError)
		return
	}
	http.Redirect(response, request, "http://localhost:3031/login_end_stage?status=success", http.StatusFound)
}

func sendErrorWithStatusCode(response http.ResponseWriter, request *http.Request, statusCode int) {
	http.Redirect(response, request, "http://localhost:3031/login_end_stage?status=failed&code="+strconv.Itoa(statusCode), http.StatusFound)
}

func sendError(response http.ResponseWriter, request *http.Request) {
	http.Redirect(response, request, "http://localhost:3031/login_end_stage?status=failed", http.StatusFound)
}
