package authentication

import (
	"authorization-module/config"
	"net/http"
	"strconv"
)

func sendErrorWithStatusCode(response http.ResponseWriter, request *http.Request, statusCode int) {
	http.Redirect(response, request, "http://"+config.MAIN_DOMAIN+"/login_end_stage?status=failed&code="+strconv.Itoa(statusCode), http.StatusFound)
}

func sendError(response http.ResponseWriter, request *http.Request) {
	http.Redirect(response, request, "http://"+config.MAIN_DOMAIN+"/login_end_stage?status=failed", http.StatusFound)
}
