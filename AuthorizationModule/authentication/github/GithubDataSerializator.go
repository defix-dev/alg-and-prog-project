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

type GithubUserDataRequired struct {
	Email string `json:"email"`
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

func DeserializeToRequiredData(formattedBody string) (*GithubUserDataRequired, error) {
	var requiredData GithubUserDataRequired
	err := json.Unmarshal([]byte(formattedBody), &requiredData)
	if err != nil {
		return nil, err
	}
	return &requiredData, nil
}
