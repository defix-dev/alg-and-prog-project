package main

import (
	mongodb "authorization-module/database"
	"context"
	"fmt"
	"log"

	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"go.mongodb.org/mongo-driver/mongo/readpref"
)

func main() {
	// Устанавливаем контекст
	ctx := context.Background()

	// Подключаемся к серверу MongoDB
	clientOptions := options.Client().ApplyURI("mongodb://localhost:27017") // Замените на ваш URI
	client, err := mongo.Connect(ctx, clientOptions)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Disconnect(ctx)

	// Проверка соединения
	err = client.Ping(ctx, readpref.Primary())
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println("Successfully connected to MongoDB!")

	// Получаем коллекцию
	db := client.Database("sigma-squad-mongo")

	//creator := mongodb.ConstructUserCreator(db)
	//finder := database.ConstructUserFinder(db)
	modificator := mongodb.ConstructUserModificator(db)

	//creator.CreateByEmail("testmail@test.com")
	modificator.ModifyNameByEmail("poncick", "testmail@test.com")
}