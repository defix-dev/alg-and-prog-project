package authentication

import (
	"authorization-module/authentication/github"
	"authorization-module/authentication/yandex"
	mongodb "authorization-module/database"
	"encoding/json"
	"fmt"
	"log"
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
		http.Redirect(response, request, "http://localhost:3030/login?error="+strconv.Itoa(http.StatusForbidden), http.StatusFound)
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

	// Проверка на nil для значений
	if accessToken == "" || authType == "" {
		http.Error(response, "Invalid request", http.StatusBadRequest)
		return
	}

	var jsonBody string
	if authType == "yandex" {
		jsonBody = yandex.ChangeCodeToUserInfo(accessToken)
	} else {
		jsonBody = github.ChangeCodeToUserInfo(accessToken)
	}

	if jsonBody == "" {
		http.Error(response, "Failed to retrieve user info", http.StatusInternalServerError)
		return
	}

	var requiredData YandexUserDataRequired
	if err := json.Unmarshal([]byte(jsonBody), &requiredData); err != nil {
		log.Println("Error unmarshalling data:", err)
		http.Error(response, "Failed to parse user data", http.StatusInternalServerError)
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
		http.Error(response, "User not found", http.StatusNotFound)
		return
	}

	permissions, err := mongodb.GeneratePermissions(user.Role)
	if err != nil {
		http.Error(response, "Failed to generate permissions", http.StatusInternalServerError)
		return
	}

	fmt.Println("Current Permissions: ")
	fmt.Println(permissions)
	http.Redirect(response, request, "http://localhost:3030/", http.StatusFound)
}
