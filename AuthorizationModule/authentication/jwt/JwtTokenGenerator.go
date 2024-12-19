package jwt_token

import (
	"time"

	"github.com/golang-jwt/jwt/v5"
)

const (
	PRIVATE_KEY = "sigma_squad_key123"
)

func GenerateAccessToken(permissions []string) (string, error) {
	return generateJwtToken(jwt.MapClaims{
		"permissions": permissions,
		"exp":         time.Now().Add(time.Minute),
	})
}

func GenerateRefreshToken(email string) (string, error) {
	return generateJwtToken(jwt.MapClaims{
		"email": email,
		"exp":   time.Now().Add(time.Hour * 24 * 7),
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
