package mongodb

type Role struct {
	Name        string   `bson:"_name"`
	Stage       int      `bson:"stage"`
	Permissions []string `bson:"permissions"`
}
