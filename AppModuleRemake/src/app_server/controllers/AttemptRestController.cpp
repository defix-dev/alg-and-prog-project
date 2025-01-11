#include "abstractions/AttemptRestController.h"
#include "Authentication.h"
#include "Server.h"
#include "Database.h"
#include "Jwt.h"

namespace Server {
    void AttemptRestController<pqxx::connection>::initialize(const std::shared_ptr<crow::SimpleApp>& app) {
        CROW_ROUTE((*app.get()), "/api/attempts/create").methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
            const char* userId = req.url_params.get("user_id");
            const char* testId = req.url_params.get("test_id");
            if(!userId || !testId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(req, m_db);
            if(!vinfo.isValid() || !Auth::AccessRequestValidator::matchTestIdSubscriber<pqxx::connection>(
                std::stoi(std::string(testId)), req, m_db
            ) || !Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(userId)), req)) {
                return crow::response(crow::status::FORBIDDEN);
            }

            try {
                auto tcheck = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId), "is_active, is_deleted");
                std::string active = tcheck[0]["is_active"], deleted = tcheck[0]["is_deleted"];
                if(active.empty() || deleted.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                if(active == "f" || deleted == "t") return crow::response(crow::status::FORBIDDEN);
                auto data = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("user_id="+std::string(userId)+" AND test_id="+std::string(testId));
                if(!data.empty()) return crow::response(crow::status::FORBIDDEN);
                std::string id = Database::Abstraction::DataUploader<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .uploadWithIdOutput(
                    Database::Postgresql::AttemptWorkpiece::get(
                        std::string(testId), std::string(userId), "", ""
                    )
                );
                if(id.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                Database::Abstraction::DataUploader<pqxx::connection> answerUp(m_db,
                Database::Configuration::Postgresql::Tables::ANSWERS);
                auto qi = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::QUESTS_TESTS)
                .getDatasByMask("test_id="+std::string(testId), "quest_id, quest_version");
                for(const auto& q : qi) {
                    std::string qid = q["quest_id"], qv = q["quest_version"];
                    if(qid.empty() || qv.empty()) continue;
                    answerUp.upload(
                        Database::Postgresql::AnswerWorkpiece::get(
                            qid, qv, id, ""
                        )
                    );
                }
                return crow::response(nlohmann::json{
                    { "attempt_id", id }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/attempts/set").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req) {
            auto j = crow::json::load(req.body);
            int attemptId = j["attempt_id"].i();
            int questId = j["quest_id"].i();
            int answerId = j["answer"].i();
            if(attemptId == 0 || questId == 0 || answerId == 0) return crow::response(crow::status::BAD_REQUEST);

            auto attempt = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
            Database::Configuration::Postgresql::Tables::ATTEMPTS).getDatasById(std::to_string(attemptId), "user_id, test_id, is_completed");
            if(attempt.empty()) return crow::response(crow::status::NOT_FOUND);
            std::string userId;
            std::string testId;
            std::string completed;
            if(userId = attempt[0]["user_id"]; userId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            if(testId = attempt[0]["test_id"]; testId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            if(completed = attempt[0]["is_completed"]; completed.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(req, m_db);
            if(!vinfo.isValid() || !Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(userId)), req)) {
                return crow::response(vinfo.getStatusCode());
            }

            try {
                auto tcheck = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId), "is_active, is_deleted");
                std::string active = tcheck[0]["is_active"], deleted = tcheck[0]["is_deleted"];
                if(active.empty() || deleted.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                if(active == "f" || deleted == "t") return crow::response(crow::status::FORBIDDEN);
                if(completed == "t") return crow::response(crow::status::FORBIDDEN);
                auto questData = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::QUESTS).getDatasById(
                    std::to_string(questId), "id, get_actual_version(id) as av"
                );
                if(questData.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                std::string av;
                if(av = questData[0]["av"]; av.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                Database::Abstraction::DataModificator<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ANSWERS).modifyByMask(
                    "question_id="+std::to_string(questId)+" AND question_version="+av+" AND attempt_id="+std::to_string(attemptId),
                    Database::Abstraction::Data({
                        Database::Abstraction::Field("answer", std::to_string(answerId))
                    })
                );
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/attempts/finish").methods(crow::HTTPMethod::PATCH)([this](const crow::request& req) {
            const char* attemptId = req.url_params.get("attempt_id");
            if(!attemptId) return crow::response(crow::status::BAD_REQUEST);

            auto attempt = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
            Database::Configuration::Postgresql::Tables::ATTEMPTS).getDatasById(std::string(attemptId), "user_id, test_id, is_completed");
            if(attempt.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            std::string userId;
            std::string testId;
            std::string completed;
            if(userId = attempt[0]["user_id"]; userId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            if(testId = attempt[0]["test_id"]; testId.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            if(completed = attempt[0]["is_completed"]; completed.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);

            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(req, m_db);
            if(!vinfo.isValid() || !Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(userId)), req)) {
                return crow::response(vinfo.getStatusCode());
            }

            try {
                auto tcheck = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId), "is_active, is_deleted");
                std::string active = tcheck[0]["is_active"], deleted = tcheck[0]["is_deleted"];
                if(active.empty() || deleted.empty()) return crow::response(crow::status::INTERNAL_SERVER_ERROR);
                if(active == "f" || deleted == "t") return crow::response(crow::status::FORBIDDEN);
                if(completed == "t") return crow::response(crow::status::FORBIDDEN);
                Database::Abstraction::DataModificator<pqxx::connection> attemptMod(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS);
                attemptMod.modify(
                    std::string(attemptId), 
                    Database::Abstraction::Data({
                        Database::Abstraction::Field("is_completed", "true")
                    })
                );
                auto answers = Database::Abstraction::DataAdapter<pqxx::connection>(m_db, 
                Database::Configuration::Postgresql::Tables::ANSWERS)
                .getDatasByMask("attempt_id="+std::string(attemptId), "answer, question_id");
                Database::Abstraction::DataAdapter<pqxx::connection> questAd(m_db,
                Database::Configuration::Postgresql::Tables::QUESTS);
                int mark = 0;
                for(const auto& answer : answers) {
                    std::string av = answer["answer"], qid = answer["question_id"];
                    if(av.empty() || qid.empty()) continue;
                    if(std::string qv = questAd.getDatasById(qid, "correct_answer")[0]["correct_answer"]; !qv.empty()) mark += (av == qv) ? 1 : 0;
                }
                attemptMod.modify(std::string(attemptId), Database::Abstraction::Data({
                    Database::Abstraction::Field("mark", std::to_string(mark))
                }));
                return crow::response(crow::status::OK);
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });

        CROW_ROUTE((*app.get()), "/api/attempts/get").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
            const char* userId = req.url_params.get("user_id");
            const char* testId = req.url_params.get("test_id");
            if(!userId || !testId) return crow::response(crow::status::BAD_REQUEST);
            auto vinfo = Auth::AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(req, m_db, 
            Server::Configuration::Permissions::Attempts::GET_ATTEMPT_INFO);

            auto course = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
            Database::Configuration::Postgresql::Tables::TESTS).getDatasById(std::string(testId), 
            "course_id");
            std::string courseId;
            if(courseId = course[0]["course_id"]; courseId.empty()) return crow::response(crow::status::NOT_FOUND);

            if(!vinfo.isValid()) {
                if(!(vinfo.getStatusCode() == crow::status::FORBIDDEN && (Auth::AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(
                    std::stoi(courseId), req, m_db
                ) || Auth::AccessRequestValidator::matchUserId(std::stoi(std::string(userId)), req)))) {
                return crow::response(vinfo.getStatusCode());
                }
            }

            try {
                auto attempt = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ATTEMPTS)
                .getDatasByMask("user_id="+std::string(userId)+" AND test_id="+std::string(testId), "id");
                std::string aid;
                if(aid = attempt[0]["id"]; aid.empty()) return crow::response(crow::status::NOT_FOUND);
                auto answers = Database::Abstraction::DataAdapter<pqxx::connection>(m_db,
                Database::Configuration::Postgresql::Tables::ANSWERS)
                .getDatasByMask("attempt_id="+aid, "answer, question_id, question_version");
                if(answers.empty()) return crow::response(crow::status::NOT_FOUND);
                auto janswers = nlohmann::json::array();
                Database::Abstraction::DataAdapter<pqxx::connection> questAd(m_db, 
                Database::Configuration::Postgresql::Tables::QUESTS);
                for(const auto& answer : answers) {
                    std::string av = answer["answer"];
                    std::string qid = answer["question_id"];
                    std::string qv = answer["question_version"];
                    auto qdata = questAd.getDatasByMask("id="+qid+" AND version="+qv, "answers["+av+"] as answerv, name, id");
                    std::string an = av != "-1" ? qdata[0]["answerv"] : "ответ еще не дан";
                    std::string qn = qdata[0]["name"];
                    if(an.empty() || qn.empty()) continue;
                    janswers.push_back(nlohmann::json{
                        { "quest_id", qid },
                        { "quest_version", qv },
                        { "quest_name", qn },
                        { "answer", an }
                    });
                }
                return crow::response(nlohmann::json{
                    { "answers", janswers }
                }.dump());
            } catch(std::exception& e) {
                CROW_LOG_DEBUG << e.what();
                return crow::response(crow::status::INTERNAL_SERVER_ERROR);
            }
        });        
    }
}