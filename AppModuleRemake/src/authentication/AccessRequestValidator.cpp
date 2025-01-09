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
            int userId = Jwt::JwtRequestParser::parse(req).getId();
            auto data = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::COURSES)
            .getDatasById(std::to_string(id), "teacher_id");
            if(!(!data.empty() && !data[0]["teacher_id"].empty())) return false;
            return true;
        } catch (std::exception&) {
            return false;
        }
    }
}