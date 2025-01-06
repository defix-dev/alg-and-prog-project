#ifndef DATABASE_INITIALIZER_H
#define DATABASE_INITIALIZER_H

#include<memory>
#include<iostream>

template<typename DB>
class DatabaseInitializer {
    public:
        bool initialized() const;
        void initialize();
        const std::shared_ptr<DB>& getConnection() const;

    private:
        std::shared_ptr<DB> m_db;
};

#endif