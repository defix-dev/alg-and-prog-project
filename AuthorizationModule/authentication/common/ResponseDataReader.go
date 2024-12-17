package common

import (
	"io/ioutil"
	"log"
	"net/http"
)

func ReadResponseData(res *http.Response) string {
	body, err := ioutil.ReadAll(res.Body)
	if err != nil {
		log.Fatal(err)
	}
	return string(body)
}
