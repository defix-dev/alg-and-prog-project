package database

import (
	jwt_token "authorization-module/authentication/jwt"
	mongodb "authorization-module/database"
	"encoding/json"
	"net/http"
	"strconv"
)

type RoleBody struct {
	Role string `json:"role"`
}

func StartHandle() {
	http.HandleFunc("/api/users/get_role", handleGetRolesPath)
	http.HandleFunc("/api/users/set_role", handleSetRolesPath)
}

func handleGetRolesPath(response http.ResponseWriter, request *http.Request) {
	accessToken := request.Header.Get("Authorization")[7:]

	if !jwt_token.ValidateToken(accessToken) {
		response.WriteHeader(http.StatusUnauthorized)
		return
	}

	decryptedToken, err := jwt_token.DecryptAccessToken(accessToken)
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		return
	}

	hasPermission := false
	for _, s := range decryptedToken.Permissions {
		if s == "user:roles:read" {
			hasPermission = true
		}
	}

	if !hasPermission {
		response.WriteHeader(http.StatusForbidden)
		return
	}

	query := request.URL.Query()
	id := query.Get("id")

	if id == "" {
		response.WriteHeader(http.StatusBadRequest)
		return
	}

	iid, err := strconv.Atoi(id)
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		return
	}

	user, err := mongodb.ConstructUserFinder().FindById(iid)
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		return
	}

	body, err := json.Marshal(RoleBody{
		Role: user.Role,
	})
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		return
	}

	response.Header().Add("Content-Type", "application/json")
	response.WriteHeader(http.StatusOK)
	response.Write(body)
}

func handleSetRolesPath(response http.ResponseWriter, request *http.Request) {
	accessToken := request.Header.Get("Authorization")[7:]

	if !jwt_token.ValidateToken(accessToken) {
		response.WriteHeader(http.StatusUnauthorized)
		return
	}

	decryptedToken, err := jwt_token.DecryptAccessToken(accessToken)
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		return
	}

	hasPermission := false
	for _, s := range decryptedToken.Permissions {
		if s == "user:roles:set" {
			hasPermission = true
		}
	}

	if !hasPermission {
		response.WriteHeader(http.StatusForbidden)
		return
	}

	query := request.URL.Query()
	id := query.Get("id")
	role := query.Get("role")

	if id == "" || role == "" {
		response.WriteHeader(http.StatusBadRequest)
		return
	}

	iid, err := strconv.Atoi(id)
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		return
	}

	err = mongodb.ConstructUserModificator().ModifyRoleById(role, iid)
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		return
	}

	response.WriteHeader(http.StatusOK)
}
