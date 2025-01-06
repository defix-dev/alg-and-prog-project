#include <iostream>
#include <cpr/cpr.h>
#include "database/postgresql/DatabaseInitializer.cpp"
#include "database/postgresql/DataUploader.cpp"
#include "database/postgresql/DataRemover.cpp"
#include "database/postgresql/DataModificator.cpp"
#include "database/postgresql/DataAdapter.cpp"

int main() {
    DatabaseInitializer<pqxx::connection> db{};
    db.initialize();
    if(!db.initialized()) return 0;

    const std::string table = "test";

    DataUploader<pqxx::connection> uploader(db.getConnection(), table);
    uploader.upload(Data({(Field("name", "defix-dev:3"))}));

    DataRemover<pqxx::connection> remover(db.getConnection(), table);
    remover.remove("1");

    DataModificator<pqxx::connection> modificator(db.getConnection(), table);
    modificator.modify("2", Data({(Field("name", "kostya(edited)"))}));

    DataAdapter<pqxx::connection> adapter(db.getConnection(), table);

    for(const auto& field : adapter.getDataById("2").getFields()) {
        std::cout << field.getName() << "=" << field.getValue() << std::endl;
    }

    return 0;
}

