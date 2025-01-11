#ifndef ANSWER_REST_CONTROLLER_H
#define ANSWER_REST_CONTROLLER_H

#include <pqxx/pqxx>
#include "../../abstractions/Controller.h"

namespace Server {
    template<typename DB>
    class AnswerRestController : public Abstraction::Controller {
        public:
            AnswerRestController(const std::shared_ptr<DB>& db) 
            : m_db(db) {}
            void initialize(const std::shared_ptr<crow::SimpleApp>& app) override;
        private:
            std::shared_ptr<DB> m_db;
    };

    template<>
    class AnswerRestController<pqxx::connection> : public Abstraction::Controller {
        public:
            AnswerRestController(const std::shared_ptr<pqxx::connection>& db) 
            : m_db(db) {}
            void initialize(const std::shared_ptr<crow::SimpleApp>& app) override;
        private:
            std::shared_ptr<pqxx::connection> m_db;
    };
}

#endif