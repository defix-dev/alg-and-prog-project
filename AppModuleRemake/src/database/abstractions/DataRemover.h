#ifndef DATA_REMOVER_H
#define DATA_REMOVER_H

#include "DatabaseUtil.h"
#include<pqxx/pqxx>

namespace Database {
    namespace Abstraction {
        template<typename DB>
        class DataRemover : private DatabaseUtil<DB> {
            public:
                DataRemover(const std::shared_ptr<DB>& db, const std::string& tableNm)
                : DatabaseUtil<DB>(db, tableNm) {}
                void remove(const std::string& id);
        };

        template<>
        class DataRemover<pqxx::connection> : private DatabaseUtil<pqxx::connection> {
            public:
                DataRemover(const std::shared_ptr<pqxx::connection>& db, const std::string& tableNm)
                : DatabaseUtil<pqxx::connection>(db, tableNm) {}
                void removeByMask(const std::string& mask);
                void remove(const std::string& id);
        };
    }
}

#endif