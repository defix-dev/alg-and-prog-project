package github

const (
	CLIENT_ID     = "Ov23liim0xQ8z7UxKcvw"
	CLIENT_SECRET = "08fc849dc9601e580a3c2d3b76ac54fb3f88d43c"
)

func CreateStageOneRef(token string) string {
	return "https://github.com/login/oauth/authorize?client_id=" + CLIENT_ID + "&state=" + token + "&scope=user:email"
}

func CreateStageTwoRef() string {
	return "https://github.com/login/oauth/access_token"
}

func CreateUserInfoRef() string {
	return "https://api.github.com/user"
}
