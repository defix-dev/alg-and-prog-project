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
            if(!Auth::AccessRequestValidator::matchUserId(id, req)) {
                return crow::response(crow::status::FORBIDDEN);
            }
            try {
                Database::Abstraction::DataUploader<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .upload(Database::Postgresql::UserWorkpiece::get(email, "", std::to_string(id), ""));
                return crow::response(crow::status::OK);
            } catch (std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_full_name")([this](const crow::request& req){
            const char* id = req.url_params.get("id");
            if(!id) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }
            try {
                CROW_LOG_DEBUG << "Database process...";
                auto data = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS).getDatasById(std::string(id));
                if(data.empty()) return crow::response(crow::status::NOT_FOUND);
                return crow::response(nlohmann::json{
                    { "full_name", data[0]["full_name"] }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << "GFN Error: " << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_users_list")([this](const crow::request& req){
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Users::GET_USERS_LIST);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }
            try {
                nlohmann::json j = nlohmann::json::array();
                auto datas = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .getDatasByMask("", "id, full_name");
                if(datas.empty()) return crow::response(crow::status::NOT_FOUND);
                for(const auto& data : datas) {
                    j.push_back(data.toJson());
                }
                return crow::response(nlohmann::json{{"users", j}}.dump());
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_user_info")([this](const crow::request& req){
            const char* id = req.url_params.get("id");
            const char* type = req.url_params.get("type");
            if(!id || !type) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Users::GET_USER_INFO);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(id)), req))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }
            try {
                if(type == "courses") {
                    auto jcourses = nlohmann::json::array();
                    auto courseIds = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
                    Database::Configuration::Postgresql::Tables::USERS_COURSES)
                    .getDatasByMask("user_id="+std::string(id), "course_id");
                    Database::Abstraction::DataAdapter<pqxx::connection> courseAd(m_db, 
                    Database::Configuration::Postgresql::Tables::COURSES);
                    for(const auto& courseId : courseIds) {
                        if(std::string cid = courseId["course_id"]; !cid.empty())
                            if(auto course = courseAd.getDatasById(cid)[0]; !course.getFields().empty())
                                jcourses.push_back(course.toJson());
                    }
                    return crow::response(nlohmann::json {
                        { "courses", jcourses }
                    }.dump());
                }
                if(type == "marks") {
                    auto jmarks = nlohmann::json::array();
                    auto marks = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
                    Database::Configuration::Postgresql::Tables::ATTEMPTS)
                    .getDatasByMask("user_id="+std::string(id) + " AND is_completed=true", "test_id, mark");
                    for(const auto& mark : marks) {
                        if(!mark.getFields().empty()) jmarks.push_back(mark.toJson());
                    }
                    return crow::response(nlohmann::json {
                        { "marks", jmarks }
                    }.dump());
                }
                if(type == "tests") {
                    auto jtests = nlohmann::json::array();
                    auto testIds = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
                    Database::Configuration::Postgresql::Tables::ATTEMPTS)
                    .getDatasByMask("user_id="+std::string(id), "test_id");
                    Database::Abstraction::DataAdapter<pqxx::connection> testAd(m_db, 
                    Database::Configuration::Postgresql::Tables::TESTS);
                    for(const auto& testId : testIds) {
                        if(std::string tid = testId["test_id"]; !tid.empty())
                            if(auto test = testAd.getDatasById(tid)[0]; !test.getFields().empty())
                                jtests.push_back(test.toJson());
                    }
                    return crow::response(nlohmann::json {
                        { "tests", jtests }
                    }.dump());
                }
                else {
                    return crow::response(crow::status::BAD_REQUEST);
                }
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/set_full_name").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req){
            const char* id = req.url_params.get("id");
            const char* name = req.url_params.get("name");
            if(!id || !name) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Users::SET_FULL_NAME);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(id)), req))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }
            try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .modify(std::string(id), Database::Abstraction::Data({Database::Abstraction::Field("full_name", name)}));
                return crow::response(crow::status::OK);
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/get_role")([this](const crow::request& req){
            const char* id = req.url_params.get("id");
            if(!id) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Users::GET_ROLE);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            std::stringstream url;
            url << "http://" << Server::Configuration::AuthServ::DOMAIN_NAME << "/api/users/get_role?id=" << id;
            cpr::Response res = cpr::Get(cpr::Url(url.str()), cpr::Header({{"Authorization", "Bearer "+req.get_header_value("Authorization").substr(7)}}));
            if(res.status_code != crow::status::OK) return crow::response(res.status_code);

            try {
                auto j = nlohmann::json::parse(res.text);
                std::string role = j["role"];
                if(role.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                return crow::response(res.text);
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/set_role").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req){
            const char* id = req.url_params.get("id");
            const char* role = req.url_params.get("role");
            if(!id || !role) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Users::SET_ROLE);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            std::stringstream url;
            url << "http://" << Server::Configuration::AuthServ::DOMAIN_NAME << "/api/users/set_role?id=" << id << "&role=" << role;
            cpr::Response res = cpr::Post(cpr::Url(url.str()), cpr::Header({{"Authorization", "Bearer "+req.get_header_value("Authorization").substr(7)}}));
            if(res.status_code != crow::status::OK) return crow::response(res.status_code);

            return crow::response(res.status_code);
        });

        CROW_ROUTE((*app.get()), "/api/users/get_block_status")([this](const crow::request& req){
            const char* id = req.url_params.get("id");
            if(!id) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Users::GET_BLOCK_STATUS);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            try {
                std::string blocked = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .getDatasById(std::string(id), "is_blocked")[0]["is_blocked"];
                if(blocked.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                return crow::response(nlohmann::json{
                    { "is_blocked", blocked }
                }.dump());
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/users/set_block_status").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req){
            const char* id = req.url_params.get("id");
            const char* block = req.url_params.get("block");
            if(!id || !block) {
                return crow::response(crow::status::BAD_REQUEST);
            }
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Users::SET_BLOCK_STATUS);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

             try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db, Database::Configuration::Postgresql::Tables::USERS)
                .modify(std::string(id), Database::Abstraction::Data({Database::Abstraction::Field("is_blocked", std::string(block))}));
                return crow::response(crow::status::OK);
            } catch(std::exception&) {
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });
    }
}