package github

import (
	"authorization-module/authentication/common"
	"net/http"
)

func ChangeCodeToUserInfo(accessToken string) string {
	req, _ := http.NewRequest("GET", CreateUserInfoRef(), nil)
	req.Header.Set("Authorization", "Bearer "+accessToken)
	res, _ := (&http.Client{}).Do(req)
	return common.ReadResponseData(res)
}
