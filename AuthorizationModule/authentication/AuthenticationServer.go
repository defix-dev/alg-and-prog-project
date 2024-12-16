package authentication

import (
	"net/http"
)

type AuthenticationServer struct {
}

func (server *AuthenticationServer) startServer() {
	http.ListenAndServe("localhost:3031", nil)
}
