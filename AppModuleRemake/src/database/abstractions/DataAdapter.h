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
                Data getDataById(const std::string& id);
        };
    }
}

#endif