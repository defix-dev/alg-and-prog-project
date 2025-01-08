package jwt_token

import (
	"time"

	"github.com/golang-jwt/jwt/v5"
)

const (
	PRIVATE_KEY = "sigma_squad_key123"
)

func GenerateAccessToken(id int, permissions []string) (string, error) {
	return generateJwtToken(jwt.MapClaims{
		"id":          id,
		"permissions": permissions,
		"exp":         time.Now().Add(time.Hour).Unix(),
	})
}

func GenerateRefreshToken(email string) (string, error) {
	return generateJwtToken(jwt.MapClaims{
		"email": email,
		"exp":   time.Now().Add(time.Hour * 24 * 7).Unix(),
	})
}

func generateJwtToken(payload jwt.MapClaims) (string, error) {
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, payload)

	outputToken, err := token.SignedString([]byte(PRIVATE_KEY))
	if err != nil {
		return "", err
	}

	return outputToken, err
}
