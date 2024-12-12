package mongodb

import (
	"context"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
)

type UserModificator struct {
	collection *mongo.Collection
	context    context.Context
}

func ConstructUserModificator(db *mongo.Database) *UserModificator {
	return &UserModificator{
		collection: db.Collection(USERS_COLLECTION),
		context:    context.Background(),
	}
}

func (modificator *UserModificator) ModifyNameByEmail(value string, email string) error {
	return modificator.modifyKeyByEmail("name", value, email)
}

func (modificator *UserModificator) ModifyRoleByEmail(value string, email string) error {
	return modificator.modifyKeyByEmail("role", value, email)
}

func (modificator *UserModificator) ModifyTokensByEmail(value bson.A, email string) error {
	return modificator.modifyKeyByEmail("tokens", value, email)
}

func (modificator *UserModificator) ModifyPhoneNumberByEmail(value string, email string) error {
	return modificator.modifyKeyByEmail("phoneNumber", value, email)
}

func (modificator *UserModificator) modifyKeyByEmail(key string, value any, email string) error {
	_, err := modificator.collection.UpdateOne(modificator.context, bson.M{"_email": email}, bson.M{
		"$set": bson.M{
			key: value,
		},
	})
	return err
}
