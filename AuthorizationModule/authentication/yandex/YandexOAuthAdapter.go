package yandex

import (
	"authorization-module/authentication/common"
	"bytes"
	"net/http"
	"net/url"
)

func GetTokensBodyByCode(code string) string {
	req, err := http.NewRequest("POST", CreateStageTwoRef(), bytes.NewBufferString(prepareDataToRequest(code).Encode()))
	req.Header.Set("Content-Type", "application/x-www-form-urlencoded")

	if err != nil {
		return "error"
	}

	res, err := (&http.Client{}).Do(req)

	if err != nil {
		return "error"
	}
	return common.ReadResponseData(res)
}

func GetAccessToken(code string) string {
	return DeserializeToDTO(GetTokensBodyByCode(code)).AccessToken
}

func prepareDataToRequest(code string) url.Values {
	output := url.Values{}
	output.Set("grant_type", "authorization_code")
	output.Set("code", code)
	output.Set("client_id", CLIENT_ID)
	output.Set("client_secret", CLIENT_SECRET)
	return output
}
