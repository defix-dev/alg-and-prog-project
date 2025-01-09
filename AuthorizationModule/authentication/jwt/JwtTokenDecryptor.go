package jwt_token

import (
	"fmt"

	"github.com/golang-jwt/jwt/v5"
)

type AccessTokenBody struct {
	Id          int
	Permissions []string
	ExpiresAt   int64
}

type RefreshTokenBody struct {
	Email     string
	ExpiresAt int64
}

func DecryptAccessToken(token string) (*AccessTokenBody, error) {
	if !ValidateToken(token) {
		return nil, fmt.Errorf("Token invalid")
	}

	parsedToken, err := jwt.Parse(token, ValidateSignMethod)
	if err != nil {
		return nil, err
	}

	var tokenBody AccessTokenBody
	if claims, ok1 := parsedToken.Claims.(jwt.MapClaims); ok1 {
		if id, ok4 := claims["id"].(int); ok4 {
			tokenBody.Id = id
		}
		if permissions, ok := claims["permissions"].([]interface{}); ok {
			for _, perm := range permissions {
				if permStr, ok := perm.(string); ok {
					tokenBody.Permissions = append(tokenBody.Permissions, permStr)
				}
			}
		}
		if exp, ok3 := claims["exp"].(int64); ok3 {
			tokenBody.ExpiresAt = exp
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
		if exp, ok3 := claims["exp"].(int64); ok3 {
			tokenBody.ExpiresAt = exp
		}
	}
	return &tokenBody, nil
}
