#include "../abstractions/Controller.h"
#include <pqxx/pqxx>
#include "../../jwt/JwtRequestValidator.cpp"
#include "../../database/postgresql/DataUploader.cpp"
#include "../../database/postgresql/workpiece/upload/UserWorkpiece.cpp"

namespace Server {
    template<typename DB>
    class UserRestController : public Abstraction::Controller {
        public:
            UserRestController(const std::shared_ptr<DB>& db) 
            : m_db(db) {}
            void initialize(const std::shared_ptr<crow::SimpleApp>& app) override;
        private:
            std::shared_ptr<DB> m_db;
    };

    template<>
    void UserRestController<pqxx::connection>::initialize(const std::shared_ptr<crow::SimpleApp>& app) {
        CROW_ROUTE((*app.get()), "/register").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
            std::string email = req.url_params.get("email");
            if(email.empty() || !Jwt::JwtRequestValidator::isValid(req)) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            try {
                Database::Abstraction::DataUploader<pqxx::connection> uploader(m_db, "users");
                uploader.upload(Database::Postgresql::UserWorkpiece::get(email, ""));
                return crow::response(crow::status::OK);
            } catch (std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });
    }
}