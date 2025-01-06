#ifndef DATA_MODIFICATOR_H
#define DATA_MODIFICATOR_H

#include "DatabaseUtil.h"
#include "Data.h"

template<typename DB>
class DataModificator : private DatabaseUtil<DB> {
    public:
        DataModificator(const std::shared_ptr<DB>& db, const std::string& tableNm)
        : DatabaseUtil<DB>(db, tableNm) {}
        void modify(const std::string& id, const Data& data);
};

#endif