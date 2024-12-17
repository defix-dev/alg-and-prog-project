package github

import (
	"encoding/json"
	"log"
	"net/url"
)

type GithubRequestDTO struct {
	ClientID     string `json:"client_id"`
	ClientSecret string `json:"client_secret"`
	Code         string `json:"code"`
	State        string `json:"state"`
}

type GithubResponseDTO struct {
	AccessToken string
	Scope       string
	TokenType   string
}

func SerializeToJson(dto GithubRequestDTO) []byte {
	body, err := json.Marshal(dto)
	if err != nil {
		log.Fatal(err)
	}
	return body
}

func DeserializeToDTO(formattedBody string) GithubResponseDTO {
	query, _ := url.ParseQuery(formattedBody)
	return GithubResponseDTO{
		AccessToken: query.Get("access_token"),
		Scope:       query.Get("scope"),
		TokenType:   query.Get("token_type"),
	}
}
