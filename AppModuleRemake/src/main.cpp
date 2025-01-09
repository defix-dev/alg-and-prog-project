#include <iostream>
#include "Server.h"
#include "Database.h"

int main() {
    Database::Abstraction::DatabaseInitializer<pqxx::connection> dbInitializer{};
    dbInitializer.initialize();

    if(!dbInitializer.initialized()) return 1;

    std::vector<std::shared_ptr<Server::Abstraction::Controller>> controllers{
        (std::make_shared<Server::UserRestController<pqxx::connection>>(dbInitializer.getConnection())),
        (std::make_shared<Server::CourseRestController<pqxx::connection>>(dbInitializer.getConnection()))
    };
    Server::Abstraction::DefaultServerInitializer servInitializer{controllers};
    servInitializer.initialize();
    return 0;
}

