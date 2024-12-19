package jwt_token

import (
	"fmt"

	"github.com/golang-jwt/jwt/v5"
)

func ValidateToken(token string) bool {
	parsedToken, err := jwt.Parse(token, ValidateSignMethod)

	if err != nil {
		return false
	}

	if _, ok := parsedToken.Claims.(jwt.MapClaims); ok && parsedToken.Valid {
		return true
	}
	return false
}

func ValidateSignMethod(jwtToken *jwt.Token) (interface{}, error) {
	if _, ok := jwtToken.Method.(*jwt.SigningMethodHMAC); !ok {
		return nil, fmt.Errorf("invalid sign method")
	}
	return []byte(PRIVATE_KEY), nil
}
