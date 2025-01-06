#ifndef DATA_REMOVER_H
#define DATA_REMOVER_H

#include "DatabaseUtil.h"

template<typename DB>
class DataRemover : private DatabaseUtil<DB> {
    public:
        DataRemover(const std::shared_ptr<DB>& db, const std::string& tableNm)
         : DatabaseUtil<DB>(db, tableNm) {}
        void remove(const std::string& id);
};

#endif