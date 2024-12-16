package yandex

import (
	"net/http"
)

type YandexAuthExecutor struct {
	writer *http.ResponseWriter
	token  string
}

func ConstructYandexAuthExecutor(writer *http.ResponseWriter, token string) *YandexAuthExecutor {
	return &YandexAuthExecutor{writer: writer, token: token}
}

func (executor *YandexAuthExecutor) Execute() {

}
