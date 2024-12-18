package mongodb

import (
	"context"
	"log"
	"strconv"

	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

var database *mongo.Database = ConstructDatabaseInitializer(27017).Database

type DatabaseInitializer struct {
	Database *mongo.Database
}

func ConstructDatabaseInitializer(port int) DatabaseInitializer {
	ctx := context.Background()
	clientOptions := options.Client().ApplyURI("mongodb://localhost:" + strconv.Itoa(port))
	client, err := mongo.Connect(ctx, clientOptions)
	if err != nil {
		log.Fatal(err)
	}
	return DatabaseInitializer{
		Database: client.Database("sigma-squad-mongo"),
	}
}
