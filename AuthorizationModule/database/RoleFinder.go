package mongodb

import (
	"context"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

const (
	ROLE_COLLECTION = "roles"
)

type RoleFinder struct {
	collection *mongo.Collection
	context    context.Context
}

func ConstructRoleFinder() *RoleFinder {
	return &RoleFinder{
		collection: database.Collection(ROLE_COLLECTION),
		context:    context.Background(),
	}
}

func (finder *RoleFinder) FindRoleByName(name string) (*Role, error) {
	var result Role
	err := finder.collection.FindOne(finder.context, bson.M{"_name": name}).Decode(&result)
	if err != nil {
		return nil, err
	}
	return &result, nil
}

func (finder *RoleFinder) FindRolesByStage(stage int) (*[]Role, error) {
	var result []Role
	cur, err := finder.collection.Find(finder.context, bson.M{"stage": stage})
	if err != nil {
		return nil, err
	}
	defer cur.Close(finder.context)
	var lasterr error
	for cur.Next(finder.context) {
		var result Role
		err := cur.Decode(&result)
		if err != nil {
			lasterr = err
			break
		}
	}
	if len(result) == 0 {
		return nil, lasterr
	}
	return &result, nil
}
