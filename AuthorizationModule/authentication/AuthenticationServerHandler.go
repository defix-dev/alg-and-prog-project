package authentication

import (
	"authorization-module/authentication/github"
	"authorization-module/authentication/yandex"
	"fmt"
	"net/http"
)

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
	token, _ := request.Cookie(TEMP_LOGIN_TOKEN)

	http.SetCookie(response, &http.Cookie{
		Name:  TEMP_LOGIN_TOKEN,
		Value: token.Value,
	})

	if authType == YANDEX_AUTH_TYPE {
		http.Redirect(response, request, yandex.CreateStageOneRef(token.Value), http.StatusFound)
	} else {
		http.Redirect(response, request, github.CreateStageOneRef(token.Value), http.StatusFound)
	}
}

func handleCallbackPath(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()

	code := query.Get("code")
	stateToCheck := query.Get("state")
	authType := query.Get("auth_type")
	state, err := request.Cookie(TEMP_LOGIN_TOKEN)

	if err != nil {
		return
	}

	if stateToCheck != state.Value {
		return
	}

	var oauthkey string
	if authType == YANDEX_AUTH_TYPE {
		oauthkey = yandex.GetAccessToken(code)
	} else {
		oauthkey = github.GetAccessToken(code, stateToCheck)
	}

	http.Redirect(response, request, "http://localhost:3031/authorize?access_token="+oauthkey+"&auth_type="+authType, http.StatusFound)
}

func handleAuthorize(response http.ResponseWriter, request *http.Request) {
	query := request.URL.Query()

	accessToken := query.Get("access_token")
	authType := query.Get("auth_type")

	if authType == "yandex" {
		fmt.Println(yandex.ChangeCodeToUserInfo(accessToken))
	} else {
		fmt.Println(github.ChangeCodeToUserInfo(accessToken))
	}
}
