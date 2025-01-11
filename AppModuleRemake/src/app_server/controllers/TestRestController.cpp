#include "abstractions/TestRestController.h"
#include "Authentication.h"
#include "Server.h"
#include "Database.h"
#include "Jwt.h"

namespace Server {
    void TestRestController<pqxx::connection>::initialize(const std::shared_ptr<crow::SimpleApp>& app) {
        CROW_ROUTE((*app.get()), "/api/tests/delete_quest").methods(crow::HTTPMethod::Delete)([this](const crow::request& req){
            const char* testId = req.url_params.get("test_id");
            const char* questId = req.url_params.get("quest_id");
            if(!testId || !questId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Tests::DELETE_QUEST);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (
                Auth::AccessRequestValidator::matchCourseIdOwner(std::stoi(courseId), req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto data = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("test_id="+std::string(testId), "id");
                if(data.empty()) return crow::response(crow::status::NOT_FOUND);
                Database::Abstraction::DataRemover<pqxx::connection>(m_db, 
                Database::Configuration::Postgresql::Tables::QUESTS_TESTS)
                .removeByMask("test_id="+std::string(testId)+" AND quest_id="+std::string(questId));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/tests/add_quest").methods(crow::HTTPMethod::Post)([this](const crow::request& req){
            auto j = crow::json::load(req.body);
            int questId = j["quest_id"].i();
            int testId = j["test_id"].i();
            int questVersion = j["quest_version"].i();
            if(questId == 0 || testId == 0 || questVersion == 0) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Tests::ADD_QUEST);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::to_string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (
                Auth::AccessRequestValidator::matchCourseIdOwner(std::stoi(courseId), req, m_db) &&
                Auth::AccessRequestValidator::matchQuestIdOwner(std::stoi(courseId), 0, req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto data = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("test_id="+std::to_string(testId), "id");
                if(!data.empty()) return crow::response(crow::status::FORBIDDEN);
                Database::Abstraction::DataUploader<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::QUESTS_TESTS)
                .upload(Database::Postgresql::QuestTestWorkpiece::get(
                    std::to_string(questId),
                    std::to_string(questVersion),
                    std::to_string(testId)
                ));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/tests/set_quest_order_info").methods(crow::HTTPMethod::Post)([this](const crow::request& req){
            auto j = crow::json::load(req.body);
            int testId = j["test_id"].i();
            std::vector<std::string> quests;
            for(const auto& quest : j["quests"]) {
                if(int id = quest["id"].i(); id > 0) quests.push_back(std::to_string(id));
            }
            if(testId == 0 || quests.empty()) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Tests::SET_QUEST_ORDER_INFO);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::to_string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (
                Auth::AccessRequestValidator::matchCourseIdOwner(std::stoi(courseId), req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                Database::Abstraction::DataModificator<pqxx::connection> questMod(m_db,
                Database::Configuration::Postgresql::Tables::QUESTS_TESTS);
                for(size_t i = 0; i < quests.size(); ++i) {
                    questMod.modifyByMask("test_id="+std::to_string(testId)+" AND quest_id="+quests[i],
                    Database::Abstraction::Data({
                        Database::Abstraction::Field("order_id", std::to_string(i+1))
                    }));
                }
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/tests/get_users_list").methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            const char* testId = req.url_params.get("test_id");
            if(!testId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Tests::GET_USERS_LIST);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (
                Auth::AccessRequestValidator::matchCourseIdOwner(std::stoi(courseId), req, m_db)))) {
                    return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto jusers = nlohmann::json::array();
                auto users = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("test_id="+std::string(testId), "user_id");
                if(users.empty()) return crow::response(crow::status::NOT_FOUND);
                for(const auto& user : users) {
                    if(std::string id = user["user_id"]; !id.empty()) jusers.push_back(std::stoi(id));
                }
                return crow::response(nlohmann::json{
                    { "ids", jusers }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/tests/get_user_marks").methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            const char* testId = req.url_params.get("test_id");
            if(!testId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Tests::GET_USER_MARKS);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            bool all = false;
            if(!vinfo.isValid()) {
                if(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(
                    std::stoi(courseId), req, m_db
                )) {
                    all = true;
                } else if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchTestIdSubscriber<pqxx::connection>(
                    std::stoi(testId), req, m_db
                ))) {
                    return crow::response(vinfo.getStatusCode());
                }
            } else {
                all = true;
            }

            try {
                auto jmarks = nlohmann::json::array();
                std::string userId = std::to_string(Jwt::JwtRequestParser::parse(req).getId());
                auto marks = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("test_id="+std::string(testId)+(!all ? " AND user_id="+userId : "")+" AND is_completed=true", "mark, user_id");
                for(const auto& mark : marks) {
                    if(std::string mv = mark["mark"]; !mv.empty()) jmarks.push_back(nlohmann::json {
                        { "user_id", mark["user_id"] },
                        { "mark", mv }
                    });
                }
                return crow::response(nlohmann::json{
                    { "marks", jmarks }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/tests/get_user_answers").methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            const char* testId = req.url_params.get("test_id");
            if(!testId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>
            (req, m_db, Server::Configuration::Permissions::Tests::GET_USER_ANSWERS);

            std::string courseId = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId),"course_id")[0]["course_id"];

            if(courseId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            bool all = false;
            if(!vinfo.isValid()) {
                if(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(
                    std::stoi(courseId), req, m_db
                )) {
                    all = true;
                } else if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && Auth::AccessRequestValidator::matchTestIdSubscriber<pqxx::connection>(
                    std::stoi(testId), req, m_db
                ))) {
                    return crow::response(vinfo.getStatusCode());
                }
            } else {
                all = true;
            }

            try {
                auto jattempts = nlohmann::json::array();
                std::string userId = std::to_string(Jwt::JwtRequestParser::parse(req).getId());
                auto data = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("test_id="+std::string(testId) + (!all ? ("user_id="+userId) : "")+" AND is_completed=true", "id, user_id");
                Database::Abstraction::DataAdapter<pqxx::connection> answerAd(m_db, 
                Database::Configuration::Postgresql::Tables::ANSWERS);
                Database::Abstraction::DataAdapter<pqxx::connection> questAd(m_db, 
                Database::Configuration::Postgresql::Tables::QUESTS);
                if(data.empty()) return crow::response(crow::status::NOT_FOUND);
                for(const auto& attempt : data) {
                    if(attempt.getFields().empty()) continue;
                    std::string aid = attempt["id"];
                    std::string uid = attempt["user_id"];
                    if(aid.empty() || uid.empty()) continue;
                    auto janswers = nlohmann::json::array();
                    auto answers = answerAd.getDatasByMask("attempt_id="+aid, "answer, question_id, question_version");
                    if(answers.empty()) continue;
                    for(const auto& answer : answers) {
                        std::string av = answer["answer"];
                        std::string qid = answer["question_id"];
                        std::string qv = answer["question_version"];
                        auto qdata = questAd.getDatasByMask("id="+qid+" AND version="+qv, "answers["+av+"] as answerv, name");
                        std::string an = qdata[0]["answerv"];
                        std::string qn = qdata[0]["name"];
                        if(an.empty() || qn.empty()) continue;
                        janswers.push_back(nlohmann::json{
                            { "quest_name", qn },
                            { "answer", an }
                        });
                    }
                    jattempts.push_back(nlohmann::json{
                        { "user_id", uid },
                        { "answers", janswers }
                    });
                }
                return crow::response(nlohmann::json {
                    {"attempts", jattempts}
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });
    }
}