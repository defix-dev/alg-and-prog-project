package github

import (
	"authorization-module/authentication/common"
	"bytes"
	"log"
	"net/http"
)

func GetTokensBodyByCode(code string, state string) string {
	body := SerializeToJson(prepareDataToRequest(code, state))
	req, err := http.NewRequest("POST", CreateStageTwoRef(), bytes.NewReader(body))
	if err != nil {
		log.Fatal(err)
	}

	req.Header.Set("Content-Type", "application/json")
	res, err := (&http.Client{}).Do(req)
	if err != nil {
		log.Fatal(err)
	}
	defer res.Body.Close()
	return common.ReadResponseData(res)
}

func GetAccessToken(code string, state string) string {
	return DeserializeToDTO(GetTokensBodyByCode(code, state)).AccessToken
}

func prepareDataToRequest(code string, state string) GithubRequestDTO {
	return GithubRequestDTO{
		ClientID:     CLIENT_ID,
		ClientSecret: CLIENT_SECRET,
		Code:         code,
		State:        state,
	}
}
