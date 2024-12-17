package authentication

import (
	"net/http"
)

func StartServer() {
	http.ListenAndServe("localhost:3031", nil)
}
