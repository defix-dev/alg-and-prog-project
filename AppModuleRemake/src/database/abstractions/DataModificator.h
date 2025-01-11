#ifndef DATA_MODIFICATOR_H
#define DATA_MODIFICATOR_H

#include "DatabaseUtil.h"
#include "Data.h"
#include<pqxx/pqxx>

namespace Database {
    namespace Abstraction {
        template<typename DB>
        class DataModificator : private DatabaseUtil<DB> {
            public:
                DataModificator(const std::shared_ptr<DB>& db, const std::string& tableNm)
                : DatabaseUtil<DB>(db, tableNm) {}
                void modify(const std::string& id, const Data& data);
        };

        template<>
        class DataModificator<pqxx::connection> : private DatabaseUtil<pqxx::connection> {
            public:
                DataModificator(const std::shared_ptr<pqxx::connection>& db, const std::string& tableNm)
                : DatabaseUtil<pqxx::connection>(db, tableNm) {}
                void modifyByMask(const std::string& mask, const Data& data);
                void modify(const std::string& id, const Data& data);
        };
    }
}

#endif