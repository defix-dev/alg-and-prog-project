#include "abstractions/UserRestController.h"
#include "../ServerConfiguration.h"
#include "crow.h"
#include "Authentication.h"
#include "Database.h"
#include "Jwt.h"
#include <cpr/cpr.h>

namespace Server {
    void UserRestController<pqxx::connection>::initialize(const std::shared_ptr<crow::SimpleApp>& app) {
        CROW_ROUTE((*app.get()), "/api/users/register").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
            auto j = crow::json::load(req.body);
            if(!j) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            int id = j["id"].i();
            std::string email = j["email"].s();
            if(id == 0 || email.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validate(req);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }
            if(!Auth::AccessRequestValidator::matchId(id, req)) {
                return crow::response(crow::status::FORBIDDEN);
            }
            try {
                Database::Abstraction::DataUploader<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .upload(Database::Postgresql::UserWorkpiece::get(email, "", std::to_string(id)));
                return crow::response(crow::status::OK);
            } catch (std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_full_name")([this](const crow::request& req){
            std::string id = req.url_params.get("id");
            if(id.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }
            try {
                return crow::response(nlohmann::json{
                    { "full_name", Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .getDatasById(id)[0]["full_name"] }
                }.dump());
            } catch(std::exception& e) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_users_list")([this](const crow::request& req){
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::GET_USERS_LIST);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }
            try {
                nlohmann::json j = nlohmann::json::array();
                for(const auto& data : Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .getDatasByMask("", "id, full_name")) {
                    j.push_back(data.toJson());
                }
                return crow::response(nlohmann::json{"users", j}.dump());
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_user_info")([this](const crow::request& req){
            std::string id = req.url_params.get("id");
            if(id.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::GET_USER_INFO);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchId(std::stoi(id), req))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }
            try {
                auto courses = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::COURSES)
                .getDatasByMask("user_id="+id);
                std::vector<std::vector<Database::Abstraction::Data>> tests;
                Database::Abstraction::DataAdapter<pqxx::connection> tadapter(m_db, Database::Configuration::Postgresql::Tables::TESTS);
                for(const auto& course : courses) {
                    tests.push_back(tadapter.getDatasByMask("course_id="+course["id"]));
                } 
                auto marks = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::COURSES)
                .getDatasByMask("user_id="+id);
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/set_full_name").methods(crow::HTTPMethod::POST)([this](const crow::request& req){
            std::string id = req.url_params.get("id");
            std::string name = req.url_params.get("name");
            if(id.empty() || name.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::SET_FULL_NAME);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchId(std::stoi(id), req))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }
            try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .modify(id, Database::Abstraction::Data({Database::Abstraction::Field("full_name", name)}));
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_roles")([this](const crow::request& req){
            std::string id = req.url_params.get("id");
            if(id.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::GET_ROLES);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            std::stringstream url;
            url << "http://" << Server::Configuration::AuthServ::DOMAIN_NAME << "/api/users/get_role?id=" << id;
            cpr::Response res = cpr::Get(cpr::Url(url.str()), cpr::Header({{"Authorization", "Bearer "+Jwt::JwtRequestParser::parse(req)}}));
            if(res.status_code != crow::status::OK) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            try {
                auto j = nlohmann::json::parse(res.text);
                std::string role = j["role"];
                if(role.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                return crow::response(res.text);
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/set_roles").methods(crow::HTTPMethod::POST)([this](const crow::request& req){
            std::string id = req.url_params.get("id");
            std::string role = req.url_params.get("role");
            if(id.empty() || role.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::SET_ROLES);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            std::stringstream url;
            url << "http://" << Server::Configuration::AuthServ::DOMAIN_NAME << "/api/users/set_role?id=" << id << "&role=" << role;
            cpr::Response res = cpr::Post(cpr::Url(url.str()), cpr::Header({{"Authorization", "Bearer "+Jwt::JwtRequestParser::parse(req)}}));
            if(res.status_code != crow::status::OK) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            return crow::response(res.status_code);
        });

        CROW_ROUTE((*app.get()), "/api/users/is_blocked")([this](const crow::request& req){
            std::string id = req.url_params.get("id");
            if(id.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::IS_BLOCKED);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            try {
                std::string blocked = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .getDatasById(id, "is_blocked")[0]["is_blocked"];
                if(blocked.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                return crow::response(nlohmann::json{
                    { "is_blocked", blocked }
                }.dump());
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/set_block_status").methods(crow::HTTPMethod::POST)([this](const crow::request& req){
            std::string id = req.url_params.get("id");
            std::string block = req.url_params.get("block");
            if(id.empty() || block.empty()) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::SET_BLOCK_STATUS);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

             try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .modify(id, Database::Abstraction::Data({Database::Abstraction::Field("is_blocked", block)}));
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });
    }
}