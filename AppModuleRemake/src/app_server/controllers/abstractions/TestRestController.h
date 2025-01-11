#ifndef TEST_REST_CONTROLLER_H
#define TEST_REST_CONTROLLER_H

#include <pqxx/pqxx>
#include "../../abstractions/Controller.h"

namespace Server {
    template<typename DB>
    class TestRestController : public Abstraction::Controller {
        public:
            TestRestController(const std::shared_ptr<DB>& db) 
            : m_db(db) {}
            void initialize(const std::shared_ptr<crow::SimpleApp>& app) override;
        private:
            std::shared_ptr<DB> m_db;
    };

    template<>
    class TestRestController<pqxx::connection> : public Abstraction::Controller {
        public:
            TestRestController(const std::shared_ptr<pqxx::connection>& db) 
            : m_db(db) {}
            void initialize(const std::shared_ptr<crow::SimpleApp>& app) override;
        private:
            std::shared_ptr<pqxx::connection> m_db;
    };
}

#endif