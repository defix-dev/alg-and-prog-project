package jwt_token

import (
	mongodb "authorization-module/database"
	"fmt"

	"github.com/golang-jwt/jwt/v5"
)

func RefreshAccessToken(refreshToken string) (string, error) {
	if !ValidateToken(refreshToken) {
		return "", fmt.Errorf("invalid token")
	}

	parsedToken, err := jwt.Parse(refreshToken, nil)

	if err != nil {
		return "", err
	}

	if claims, ok1 := parsedToken.Claims.(jwt.MapClaims); ok1 {
		if email, ok2 := claims["email"].(string); ok2 {
			user, err := mongodb.ConstructUserFinder().FindByEmail(email)
			if err != nil {
				return "", err
			}
			permissions, err := mongodb.GeneratePermissions(user.Role)
			if err != nil {
				return "", err
			}
			accessToken, err := GenerateAccessToken(permissions)
			if err != nil {
				return "", err
			}
			return accessToken, nil
		}
	}
	return "", fmt.Errorf("udentified error")
}
