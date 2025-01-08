package jwt_token

import (
	"fmt"
	"time"

	"github.com/golang-jwt/jwt/v5"
)

type AccessTokenBody struct {
	Id          int
	Permissions []string
	ExpiresAt   time.Time
}

type RefreshTokenBody struct {
	Email     string
	ExpiresAt time.Time
}

func DecryptAccessToken(token string) (*AccessTokenBody, error) {
	if !ValidateToken(token) {
		return nil, fmt.Errorf("Token invalid")
	}

	parsedToken, err := jwt.Parse(token, nil)
	if err != nil {
		return nil, err
	}

	var tokenBody AccessTokenBody
	if claims, ok1 := parsedToken.Claims.(jwt.MapClaims); ok1 {
		if permissions, ok2 := claims["permissions"].([]string); ok2 {
			tokenBody.Permissions = permissions
		}
		if exp, ok3 := claims["exp"].(time.Time); ok3 {
			tokenBody.ExpiresAt = exp
		}
		if id, ok4 := claims["id"].(int); ok4 {
			tokenBody.Id = id
		}
	}
	return &tokenBody, nil
}

func DecryptRefreshToken(token string) (*RefreshTokenBody, error) {
	if !ValidateToken(token) {
		return nil, fmt.Errorf("Token invalid")
	}

	parsedToken, err := jwt.Parse(token, nil)
	if err != nil {
		return nil, err
	}

	var tokenBody RefreshTokenBody
	if claims, ok1 := parsedToken.Claims.(jwt.MapClaims); ok1 {
		if email, ok2 := claims["email"].(string); ok2 {
			tokenBody.Email = email
		}
		if exp, ok3 := claims["exp"].(time.Time); ok3 {
			tokenBody.ExpiresAt = exp
		}
	}
	return &tokenBody, nil
}
