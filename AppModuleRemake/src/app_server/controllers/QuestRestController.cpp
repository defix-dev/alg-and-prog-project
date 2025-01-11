#include "abstractions/QuestRestController.h"
#include "Authentication.h"
#include "Server.h"
#include "Database.h"
#include "Jwt.h"

namespace Server {
    void QuestRestController<pqxx::connection>::initialize(const std::shared_ptr<crow::SimpleApp>& app) {
        CROW_ROUTE((*app.get()), "/api/quests/get_quests_list")([this](const crow::request& req) {
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Quests::GET_QUESTS_LIST);
            bool all = false;
            if(!vinfo.isValid()) {
                if(int code = vinfo.getStatusCode(); code == crow::status::PAYLOAD_TOO_LARGE) return crow::response(code);
            }
            else {
                all = true;
            }

            try {
                auto jquests = nlohmann::json::array();
                Database::Abstraction::DataAdapter<pqxx::connection> questAd(m_db,
                Database::Configuration::Postgresql::Tables::QUESTS);
                std::vector<Database::Abstraction::Data> quests;
                if(all) {
                    quests = questAd.getDatasByMask("");
                } else {
                    int userId = Jwt::JwtRequestParser::parse(req).getId();
                    quests = questAd.getDatasByMask("author_id="+std::to_string(userId));
                }
                for(const auto& quest : quests) {
                    if(!quest.getFields().empty()) jquests.push_back(quest.toJson());
                }
                return crow::response(nlohmann::json{
                    { "quests", jquests }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/quests/get_quest_info")([this](const crow::request& req) {
            const char* id = req.url_params.get("id");
            const char* version = req.url_params.get("version");
            if(!id || !version) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Quests::GET_QUEST_INFO);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (Auth::AccessRequestValidator::matchAttemptExist<pqxx::connection>(std::stoi(std::string(id)), std::stoi(std::string(version)), req, m_db) ||
                Auth::AccessRequestValidator::matchQuestIdOwner(std::stoi(std::string(id)), std::stoi(std::string(version)), req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto data = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
                Database::Configuration::Postgresql::Tables::QUESTS)
                .getDatasByMask("id="+std::string(id)+" AND version="+std::string(version));
                if(data.empty()) return crow::response(crow::status::NOT_FOUND);
                return crow::response(nlohmann::json{
                    { "quest_info", data[0].toJson() }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        
        CROW_ROUTE((*app.get()), "/api/quests/set_quest_info").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
            auto j = crow::json::load(req.body);
            int questId = j["quest_id"].i();
            std::string name = j["name"].s();
            std::string text = j["text"].s();
            std::stringstream answers;
            answers << "ARRAY[";
            bool first = true;
            for(const auto& answer : j["answers"]) {
                if(!first) answers << ",";
                first = false;
                answers << "\'" << answer << "\'";
            }
            answers << "]";
            std::string answersP = answers.str();
            int correctAnswerId = j["correct_answer_id"].i();
            if(questId == 0 || name.empty() || text.empty() || answersP.empty() || correctAnswerId == 0) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Quests::SET_QUEST_INFO);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (
                Auth::AccessRequestValidator::matchQuestIdOwner(questId, 0, req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataUploader<pqxx::connection>(m_db, 
                Database::Configuration::Postgresql::Tables::QUESTS).upload(
                    Database::Postgresql::QuestWorkpiece::get(
                        name,
                        text,
                        std::to_string(Jwt::JwtRequestParser::parse(req).getId()),
                        answersP,
                        std::to_string(correctAnswerId),
                        "",
                        ""
                    )
                );
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/quests/create_quest").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
            auto j = crow::json::load(req.body);
            std::string name = j["name"].s();
            std::string text = j["text"].s();
            std::stringstream answers;
            answers << "ARRAY[";
            bool first = true;
            for(const auto& answer : j["answers"]) {
                if(!first) answers << ",";
                first = false;
                answers << "\'" << answer.s() << "\'";
            }
            answers << "]";
            std::string answersP = answers.str();
            int correctAnswerId = j["correct_answer_id"].i();
            if(name.empty() || text.empty() || answersP.empty() || correctAnswerId == 0) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Quests::CREATE_QUEST);
            if(!vinfo.isValid()) {
                return crow::response(vinfo.getStatusCode());
            }

            try {
                std::string id = Database::Abstraction::DataUploader<pqxx::connection>(m_db, 
                Database::Configuration::Postgresql::Tables::QUESTS).uploadWithIdOutput(
                    Database::Postgresql::QuestWorkpiece::get(
                        name,
                        text,
                        std::to_string(Jwt::JwtRequestParser::parse(req).getId()),
                        answersP,
                        std::to_string(correctAnswerId),
                        "",
                        ""
                    )
                );
                if(id.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                return crow::response(nlohmann::json{
                    { "quest_id", id }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/quests/delete_quest").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req) {
            const char* id = req.url_params.get("id");
            const char* version = req.url_params.get("version");
            if(!id || !version) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Quests::DELETE_QUEST);
            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (
                Auth::AccessRequestValidator::matchQuestIdOwner(std::stoi(std::string(id)), std::stoi(std::string(version)), req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataModificator<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::QUESTS).modifyByMask("id="+std::string(id)+" AND version="+std::string(version),
                Database::Abstraction::Data({
                    Database::Abstraction::Field("is_deleted", "true")
                }));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });
    }
}