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
	userId, err := creator.collection.CountDocuments(creator.context, bson.D{})
	if err != nil {
		return err
	}
	_, err = creator.collection.InsertOne(creator.context, bson.M{
		"userId":      userId + 1,
		"email":       email,
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
