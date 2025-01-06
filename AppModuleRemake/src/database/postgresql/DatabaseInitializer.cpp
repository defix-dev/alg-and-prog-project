#include "../abstractions/DatabaseInitializer.h"
#include "../DatabaseConfiguration.h"
#include <pqxx/pqxx>
#include <iostream>

template<>
bool DatabaseInitializer<pqxx::connection>::initialized() const {
    return m_db != nullptr && m_db->is_open();
}

template<>
const std::shared_ptr<pqxx::connection>& DatabaseInitializer<pqxx::connection>::getConnection() const {
    return m_db;
}

template<>
void DatabaseInitializer<pqxx::connection>::initialize() {
    std::stringstream ss;
    ss << "dbname=" << DatabaseConfiguration::DB_NAME << " port=" << DatabaseConfiguration::PORT << " user=" << DatabaseConfiguration::USER << " password=" << DatabaseConfiguration::PASS << " host=localhost";
    m_db = std::make_shared<pqxx::connection>(ss.str());
}

