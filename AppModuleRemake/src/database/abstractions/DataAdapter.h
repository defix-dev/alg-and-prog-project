#ifndef DATA_ADAPTER_H
#define DATA_ADAPTER_H

#include "DatabaseUtil.h"
#include "Data.h"

namespace Database {
    namespace Abstraction {
        template<typename DB>
        class DataAdapter : public DatabaseUtil<DB> {
            public:
                DataAdapter(const std::shared_ptr<DB>& db, const std::string& tableNm) 
                : DatabaseUtil<DB>(db, tableNm) {}
                std::vector<Data> getDatasByMask(const std::string& mask, const std::string& pattern = "*");
                std::vector<Data> getDatasById(const std::string& id, const std::string& pattern = "*");
        };

        template<>
        class DataAdapter<pqxx::connection> : public DatabaseUtil<pqxx::connection> {
        public:
            DataAdapter(const std::shared_ptr<pqxx::connection>& db, const std::string& tableNm)
            : DatabaseUtil<pqxx::connection>(db, tableNm) {}
            std::vector<Data> getDatasByMask(const std::string& mask, const std::string& pattern = "*");
            std::vector<Data> getDatasById(const std::string& id, const std::string& pattern = "*");
        };
    }
}

#endif