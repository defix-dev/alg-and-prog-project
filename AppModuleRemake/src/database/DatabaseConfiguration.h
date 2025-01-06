#ifndef DATABASE_CONFIGURATION_H
#define DATABASE_CONFIGURATION_H

struct DatabaseConfiguration
{
    static constexpr int PORT = 8080;
    static constexpr const char* USER = "postgres";
    static constexpr const char* PASS = "root";
    static constexpr const char* DB_NAME = "sigma-squad-postgresql";
};

#endif