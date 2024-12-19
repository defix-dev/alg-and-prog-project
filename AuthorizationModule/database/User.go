package mongodb

type User struct {
	Email       string       `bson:"_email"`
	Name        string       `bson:"name"`
	PhoneNumber string       `bson:"phoneNumber"`
	Role        string       `bson:"role"`
	Tokens      TokenDetails `bson:"tokens"`
}

type TokenDetails struct {
	RefreshToken string `bson:"refresh_token"`
	AuthToken    string `bson:"auth_token"`
}
