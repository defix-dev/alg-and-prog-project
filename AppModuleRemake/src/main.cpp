#include <iostream>
#include "app_server/DefaultServerInitializer.cpp"
#include "app_server/controllers/HomeController.cpp"
#include "app_server/controllers/UserRestController.cpp"
#include "database/postgresql/DatabaseInitializer.cpp"

int main() {
    Database::Abstraction::DatabaseInitializer<pqxx::connection> dbInitializer{};
    dbInitializer.initialize();

    if(!dbInitializer.initialized()) return 1;

    std::vector<std::shared_ptr<Server::Abstraction::Controller>> controllers{
        (std::make_shared<Server::HomeController>()),
        (std::make_shared<Server::UserRestController<pqxx::connection>>(dbInitializer.getConnection()))
    };
    Server::DefaultServerInitializer servInitializer{controllers};
    servInitializer.initialize();
    return 0;
}

