#ifndef DATA_ADAPTER_H
#define DATA_ADAPTER_H

#include "DatabaseUtil.h"
#include "Data.h"

template<typename DB>
class DataAdapter : private DatabaseUtil<DB> {
    public:
        DataAdapter(const std::shared_ptr<DB>& db, const std::string& tableNm) 
        : DatabaseUtil<DB>(db, tableNm) {}
        Data getDataById(const std::string& id);
};

#endif