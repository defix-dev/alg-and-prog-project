package yandex

const (
	CLIENT_ID     = "997a17c128e84f71b43ff34634971405"
	CLIENT_SECRET = "b9813125eb2d4ae4a3a7d104d2a15997"
)

func CreateStageOneRef(token string) string {
	return "https://oauth.yandex.ru/authorize?response_type=code&client_id=" + CLIENT_ID + "&state=" + token
}
