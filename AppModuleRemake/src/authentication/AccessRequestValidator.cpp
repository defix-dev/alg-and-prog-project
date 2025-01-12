#include "abstractions/AccessRequestValidator.h"
#include "Database.h"
#include "Jwt.h"

namespace Auth {
    ValidateInfo AccessRequestValidator::validate(const crow::request& req, const std::string& permission) {
        CROW_LOG_DEBUG << "Start validating...";
        if(!Jwt::JwtRequestValidator::isValid(req)) return ValidateInfo(401, false);
        CROW_LOG_DEBUG << "One Stage Validating Passed";
        if(permission.empty()) return ValidateInfo(200, true);
        CROW_LOG_DEBUG << "Parsing token...";
        auto permissions = Jwt::JwtRequestParser::parse(req).getPermissions();
        CROW_LOG_DEBUG << "Permission Adapted";
        return (std::find(permissions.begin(), permissions.end(), permission) != permissions.end()) 
        ? ValidateInfo(200, true) : ValidateInfo(403, false);
    }

    template<>
    ValidateInfo AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(const crow::request& req, const std::shared_ptr<pqxx::connection>& db, const std::string& permission) {
        try {
            CROW_LOG_DEBUG << "Start validating with block handle...";
            std::string id = std::to_string(Jwt::JwtRequestParser::parse(req).getId());
            CROW_LOG_DEBUG << "Parsed used id: " << id;
            auto data = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::USERS)
            .getDatasById(id, "is_blocked");
            if(data.empty()) return ValidateInfo(500, false);
            bool isBlocked = data[0]["is_blocked"] == "t";
            CROW_LOG_DEBUG << "Blocked status: " << isBlocked;
            if(isBlocked) return ValidateInfo(413, false);
            return AccessRequestValidator::validate(req, permission);
        } catch (std::exception& e) {
            CROW_LOG_DEBUG << "VWBH Error: " << e.what();
            return ValidateInfo(500, false);
        }
    }

    bool AccessRequestValidator::matchUserId(int id, const crow::request& req) {
        try {
            CROW_LOG_DEBUG << "Start id matching...";
            int did = Jwt::JwtRequestParser::parse(req).getId();
            CROW_LOG_DEBUG << "Success";
            return did == id;
        } catch (std::exception&) {
            return false;
        }
    }

    template<>
    bool AccessRequestValidator::matchCourseIdSubscriber<pqxx::connection>(int id, const crow::request& req, const std::shared_ptr<pqxx::connection>& db) {
        try {
            int userId = Jwt::JwtRequestParser::parse(req).getId();
            auto data = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::USERS_COURSES)
            .getDatasByMask("user_id="+std::to_string(userId)+" AND "+"course_id="+std::to_string(id), "user_id");
            if(!(!data.empty() && !data[0]["user_id"].empty())) return false;
            return true;
        } catch (std::exception&) {
            return false;
        }
    }

    template<>
    bool AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(int id, const crow::request& req, const std::shared_ptr<pqxx::connection>& db) {
        try {
            CROW_LOG_DEBUG << "Start course matching...";
            int userId = Jwt::JwtRequestParser::parse(req).getId();
            CROW_LOG_DEBUG << "Checking course db...";
            auto data = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::COURSES)
            .getDatasById(std::to_string(id), "teacher_id");
            CROW_LOG_DEBUG << "Checking data from courses...";
            if(data.empty()) return false;
            if(!(!data.empty() && !data[0]["teacher_id"].empty())) return false;
            return true;
        } catch (std::exception&) {
            return false;
        }
    }

    template<>
    bool AccessRequestValidator::matchAttemptExist<pqxx::connection>(int id, int version, const crow::request& req, const std::shared_ptr<pqxx::connection>& db) {
        try {
            auto attemptIds = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::ANSWERS)
            .getDatasByMask("question_id="+std::to_string(id)+" AND question_version="+std::to_string(version), "attempt_id");
            int userId = Jwt::JwtRequestParser::parse(req).getId();
            Database::Abstraction::DataAdapter<pqxx::connection> attemptAd(db, Database::Configuration::Postgresql::Tables::ATTEMPTS);
            std::string testId;
            for(const auto& attemptId : attemptIds) {
                if(std::string aid = attemptId["attemptId"]; !aid.empty())
                    if(auto attempt = attemptAd.getDatasByMask("id="+aid+" AND user_id="+std::to_string(userId), "test_id"); !attempt.empty())
                        if(std::string ctid = attempt[0]["test_id"]; !ctid.empty()) { testId = ctid; break; }
            }
            auto data = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::QUESTS_TESTS)
            .getDatasByMask("test_id="+testId+" AND quest_id="+std::to_string(id));
            return !data.empty();
        } catch(std::exception&) {
            return false;
        }
    }

    template<>
    bool AccessRequestValidator::matchQuestIdOwner<pqxx::connection>(int id, int version, const crow::request& req, const std::shared_ptr<pqxx::connection>& db) {
        try {
            if(version == 0) version = 1;
            CROW_LOG_DEBUG << "Start quest checker...";
            auto data = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::QUESTS)
            .getDatasByMask("id="+std::to_string(id)+" AND version="+std::to_string(version), "author_id");
            std::string authorId;
            CROW_LOG_DEBUG << "Getting author_id...";
            if(data.empty()) return false;
            if(authorId = data[0]["author_id"]; !(!data.empty() && !authorId.empty())) return false;
            CROW_LOG_DEBUG << "Checking more...";
            return authorId == std::to_string(Jwt::JwtRequestParser::parse(req).getId());
        } catch(std::exception&) {
            return true;
        }
    }

    template<>
    bool AccessRequestValidator::matchTestIdSubscriber<pqxx::connection>(int id, const crow::request& req, const std::shared_ptr<pqxx::connection>& db) {
        try {
            auto data = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::TESTS)
            .getDatasById(std::to_string(id), "course_id");
            if(data.empty()) return false;
            std::string cid = data[0]["course_id"];
            if(cid.empty()) return false;
            return matchCourseIdSubscriber(std::stoi(cid), req, db);
        } catch(std::exception&) {
            return false;
        }
    }
}