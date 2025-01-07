#ifndef DATA_UPLOADER_H
#define DATA_UPLOADER_H

#include "DatabaseUtil.h"
#include "Data.h"

namespace Database {
    namespace Abstraction {
        template<typename DB>
        class DataUploader : private DatabaseUtil<DB> {
            public:
                DataUploader(const std::shared_ptr<DB>& db, const std::string& tableNm)
                : DatabaseUtil<DB>(db, tableNm) {}
                void upload(const Data& data);
        };
    }
}

#endif