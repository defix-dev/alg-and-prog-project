package mongodb

import (
	"context"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

type UserFinder struct {
	collection mongo.Collection
	context    context.Context
}

func ConstructUserFinder(db *mongo.Database) *UserFinder {
	return &UserFinder{
		collection: *db.Collection(USERS_COLLECTION),
		context:    context.Background(),
	}
}

func (finder *UserFinder) FindByEmail(email string) (*User, error) {
	var result User
	err := finder.collection.FindOne(finder.context, bson.D{{"_email", email}}).Decode(&result)
	if err != nil {
		return nil, err
	}
	return &result, nil
}
