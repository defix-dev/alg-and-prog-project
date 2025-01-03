package yandex

import (
	"encoding/json"
	"log"
)

type YandexResponseDTO struct {
	AccessToken  string `json:"access_token"`
	ExpiresIn    int    `json:"expires_in"`
	RefreshToken string `json:"refresh_token"`
	TokenType    string `json:"token_type"`
}

type YandexUserDataRequired struct {
	Email string `json:"default_email"`
}

func DeserializeToDTO(formattedBody string) YandexResponseDTO {
	var output YandexResponseDTO
	if err := json.Unmarshal([]byte(formattedBody), &output); err != nil {
		log.Fatal(err)
	}
	return output
}

func DeserializeToRequiredData(formattedBody string) (*YandexUserDataRequired, error) {
	var requiredData YandexUserDataRequired
	err := json.Unmarshal([]byte(formattedBody), &requiredData)
	if err != nil {
		return nil, err
	}
	return &requiredData, nil
}
