#include "abstractions/AccessRequestValidator.h"
#include "Jwt.h"
#include "Database.h"

namespace Auth {
    ValidateInfo AccessRequestValidator::validate(const crow::request& req, const std::string& permission) {
        if(!Jwt::JwtRequestValidator::isValid(req)) return ValidateInfo(401, false);
        if(permission.empty()) return ValidateInfo(200, true);
        std::string token = Jwt::JwtRequestParser::parse(req);
        auto permissions = Jwt::JwtAdapter::getAccessTokenBody(token).getPermissions();
        return (std::find(permissions.begin(), permissions.end(), permission) != permissions.end()) 
        ? ValidateInfo(200, true) : ValidateInfo(403, false);
    }

    template<>
    ValidateInfo AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(const crow::request& req, const std::shared_ptr<pqxx::connection>& db, const std::string& permission) {
        try {
        std::string id = std::to_string(Jwt::JwtAdapter::getAccessTokenBody(Jwt::JwtRequestParser::parse(req)).getId());
        bool isBlocked = Database::Abstraction::DataAdapter<pqxx::connection>(db, Database::Configuration::Postgresql::Tables::USERS)
        .getDatasById(id, "is_blocked")[0]["is_blocked"] == "true" ? true : false;
        if(!isBlocked) return ValidateInfo(413, false);
        return AccessRequestValidator::validate(req, permission);
        } catch (std::exception&) {
            return ValidateInfo(500, false);
        }
    }

    bool AccessRequestValidator::matchId(int id, const crow::request& req) {
        try {
            std::string token = Jwt::JwtRequestParser::parse(req);
            int did = Jwt::JwtAdapter::getAccessTokenBody(token).getId();
            return did == id;
        } catch (std::exception&) {
            return false;
        }
    }
}