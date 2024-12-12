package mongodb

type User struct {
	Email       string   `bson:"_email"`
	Name        string   `bson:"name"`
	PhoneNumber string   `bson:"phoneNumber"`
	Role        string   `bson:"role"`
	Tokens      []string `bson:"tokens"`
}
