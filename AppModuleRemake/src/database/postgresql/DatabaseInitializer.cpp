#include "../abstractions/DatabaseInitializer.h"
#include "Postgresql.h"
#include <pqxx/pqxx>
#include <iostream>

namespace Database {
    namespace Abstraction {
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
            ss << "dbname=" << Configuration::Postgresql::DB_NAME << " port=" << Configuration::Postgresql::PORT << " user=" << Configuration::Postgresql::USER << " password=" << Configuration::Postgresql::PASS << " host=localhost";
            m_db = std::make_shared<pqxx::connection>(ss.str());
        }
    }
}
