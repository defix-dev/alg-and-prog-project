package mongodb

import (
	"context"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

type UserRemover struct {
	collection *mongo.Collection
	context    context.Context
}

func ConstructUserRemover() *UserRemover {
	return &UserRemover{
		collection: database.Collection(USERS_COLLECTION),
		context:    context.Background(),
	}
}

func (remover *UserRemover) RemoveById(id int) error {
	_, err := remover.collection.DeleteOne(remover.context, bson.M{"userId": id})
	return err
}
