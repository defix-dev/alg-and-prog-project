#ifndef DATABASE_UTIL_H
#define DATABASE_UTIL_H

#include<memory>
#include<iostream>

namespace Database {
    namespace Abstraction {
        template<typename DB>
        class DatabaseUtil {
            public:
                DatabaseUtil(const std::shared_ptr<DB>& db, const std::string& tableNm)
                : m_db(db), m_tableNm(tableNm) {};
            protected:
                std::shared_ptr<DB> m_db;
                std::string m_tableNm;
        };
    }
}

#endif