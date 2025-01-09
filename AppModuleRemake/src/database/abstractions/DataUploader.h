#ifndef DATA_UPLOADER_H
#define DATA_UPLOADER_H

#include "DatabaseUtil.h"
#include "Data.h"
#include<pqxx/pqxx>

namespace Database {
    namespace Abstraction {
        template<typename DB>
        class DataUploader : private DatabaseUtil<DB> {
            public:
                DataUploader(const std::shared_ptr<DB>& db, const std::string& tableNm)
                : DatabaseUtil<DB>(db, tableNm) {}
                void upload(const Data& data);
                std::string uploadWithIdOutput(const Data& data);
        };

        template<>
        class DataUploader<pqxx::connection> : private DatabaseUtil<pqxx::connection> {
            public:
                DataUploader(const std::shared_ptr<pqxx::connection>& db, const std::string& tableNm)
                : DatabaseUtil<pqxx::connection>(db, tableNm) {}
                void upload(const Data& data);
                std::string uploadWithIdOutput(const Data& data);
            private:
                std::unique_ptr<pqxx::result> uploadWithResultOutput(const Data& data, bool returning = true);
        };
    }
}

#endif