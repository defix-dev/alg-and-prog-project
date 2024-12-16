package authentication

import (
	"authorization-module/authentication/github"
	"authorization-module/authentication/yandex"
	"net/http"
)

type AuthenticationServerHandler struct {
	server *AuthenticationServer
}

func ContructAuthenticationServerHandler(server *AuthenticationServer) *AuthenticationServerHandler {
	return &AuthenticationServerHandler{server: server}
}

func (handler *AuthenticationServerHandler) startHandle() {
	http.HandleFunc("/login", handleLoginPath)
	http.HandleFunc("/callback", handleCallbackPath)
}

func getCookieHandler(response http.ResponseWriter, request *http.Request) {

}

func handleCallbackPath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()

	code := query.Get("code")
	stateToCheck := query.Get("state")

	state, err := request.Cookie("state")

	if err != nil {
		return
	}

	if stateToCheck != state.Value {
		return
	}

	http.SetCookie(response, &http.Cookie{
		Name:   "code",
		Value:  code,
		MaxAge: 3600,
	})
}

func handleLoginPath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()

	authType := query.Get("type")
	token := query.Get("token")

	http.SetCookie(response, &http.Cookie{
		Name:   "state",
		Value:  token,
		MaxAge: 3600,
	})

	response.WriteHeader(http.StatusFound)

	if authType == "yandex" {
		handleByYandexPattern(&response, token)
	} else {
		handleByGithubPattern(&response, token)
	}
}

func handleByYandexPattern(response *http.ResponseWriter, token string) {
	(*response).Header().Add("Location", yandex.CreateStageOneRef(token))
}

func handleByGithubPattern(response *http.ResponseWriter, token string) {
	(*response).Header().Add("Location", github.CreateStageOneRef(token))
}
