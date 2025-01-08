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

func ConstructUserModificator() *UserModificator {
	return &UserModificator{
		collection: database.Collection(USERS_COLLECTION),
		context:    context.Background(),
	}
}

func (modificator *UserModificator) ModifyTokensByEmail(details TokenDetails, email string) error {
	_, err := modificator.collection.UpdateOne(modificator.context, bson.M{"_email": email}, bson.M{
		"$set": bson.M{
			"tokens": details,
		},
	})
	return err
}

func (modificator *UserModificator) ModifyNameByEmail(value string, email string) error {
	return modificator.modifyKeyByEmail("name", value, email)
}

func (modificator *UserModificator) ModifyRoleByEmail(value string, email string) error {
	return modificator.modifyKeyByEmail("role", value, email)
}

func (modificator *UserModificator) ModifyPhoneNumberByEmail(value string, email string) error {
	return modificator.modifyKeyByEmail("phoneNumber", value, email)
}

func (modificator *UserModificator) modifyKeyByEmail(key string, value any, email string) error {
	_, err := modificator.collection.UpdateOne(modificator.context, bson.M{"email": email}, bson.M{
		"$set": bson.M{
			key: value,
		},
	})
	return err
}

func (modificator *UserModificator) ModifyTokensById(details TokenDetails, email string) error {
	_, err := modificator.collection.UpdateOne(modificator.context, bson.M{"_email": email}, bson.M{
		"$set": bson.M{
			"tokens": details,
		},
	})
	return err
}

func (modificator *UserModificator) ModifyNameById(value string, id int) error {
	return modificator.modifyKeyById("name", value, id)
}

func (modificator *UserModificator) ModifyRoleById(value string, id int) error {
	return modificator.modifyKeyById("role", value, id)
}

func (modificator *UserModificator) ModifyPhoneNumberById(value string, id int) error {
	return modificator.modifyKeyById("phoneNumber", value, id)
}

func (modificator *UserModificator) modifyKeyById(key string, value any, id int) error {
	_, err := modificator.collection.UpdateOne(modificator.context, bson.M{"userId": id}, bson.M{
		"$set": bson.M{
			key: value,
		},
	})
	return err
}
