package github

import (
	"net/http"
)

type GithubAuthExecutor struct {
	writer *http.ResponseWriter
	token  string
}

func ConstructGithubAuthExecutor(writer *http.ResponseWriter, token string) *GithubAuthExecutor {
	return &GithubAuthExecutor{writer: writer, token: token}
}

func (executor *GithubAuthExecutor) Execute() {

}
