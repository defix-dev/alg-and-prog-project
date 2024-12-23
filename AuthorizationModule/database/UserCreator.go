package mongodb

import (
	"context"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

type UserCreator struct {
	collection *mongo.Collection
	context    context.Context
}

func ConstructUserCreator() *UserCreator {
	return &UserCreator{
		collection: database.Collection(USERS_COLLECTION),
		context:    context.Background(),
	}
}

func (creator *UserCreator) CreateByEmail(email string) error {
	_, err := creator.collection.InsertOne(creator.context, bson.M{
		"_email":      email,
		"name":        DEFAULT_NAME,
		"phoneNumber": DEFAULT_PHONE_NUMBER,
		"role":        DEFAULT_ROLE,
		"tokens": TokenDetails{
			RefreshToken: "",
			AuthToken:    "",
		},
	})
	return err
}
