#include "../abstractions/DatabaseInitializer.h"
#include "Postgresql.h"
#include <iostream>

namespace Database {
    namespace Abstraction {
        bool DatabaseInitializer<pqxx::connection>::initialized() const {
            return m_db != nullptr && m_db->is_open();
        }

        const std::shared_ptr<pqxx::connection>& DatabaseInitializer<pqxx::connection>::getConnection() const {
            return m_db;
        }
    
        void DatabaseInitializer<pqxx::connection>::initialize() {
            std::stringstream ss;
            ss << "dbname=" << Configuration::Postgresql::Run::DB_NAME << " port=" << Configuration::Postgresql::Run::PORT << " user=" << Configuration::Postgresql::Run::USER << " password=" << Configuration::Postgresql::Run::PASS << " host=localhost";
            m_db = std::make_shared<pqxx::connection>(ss.str());
        }
    }
}
