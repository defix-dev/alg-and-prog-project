package mongodb

import (
	"context"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

type UserFinder struct {
	collection *mongo.Collection
	context    context.Context
}

func ConstructUserFinder() *UserFinder {
	return &UserFinder{
		collection: database.Collection(USERS_COLLECTION),
		context:    context.Background(),
	}
}

func (finder *UserFinder) FindByEmail(email string) (*User, error) {
	return finder.Find("email", email)
}

func (finder *UserFinder) FindById(id int) (*User, error) {
	return finder.Find("userId", id)
}

func (finder *UserFinder) Find(key string, value any) (*User, error) {
	var result User
	err := finder.collection.FindOne(finder.context, bson.D{{key, value}}).Decode(&result)
	if err != nil {
		return nil, err
	}
	return &result, nil
}
