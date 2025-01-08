#ifndef ACCESS_REQUEST_VALIDATOR_H
#define ACCESS_REQUEST_VALIDATOR_H

#include <pqxx/pqxx>
#include "ValidateInfo.h"
#include "crow.h"

namespace Auth {
    class AccessRequestValidator {
        public:
            static ValidateInfo validate(const crow::request& req, const std::string& permission = "");
            static bool matchId(int id, const crow::request& req);

            template<typename DB>
            static ValidateInfo validateWithBlockHandle(const crow::request& req, const std::shared_ptr<DB>& db, const std::string& permission = "");
    };

    template<>
    ValidateInfo AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(const crow::request& req, const std::shared_ptr<pqxx::connection>& db, const std::string& permission);
}

#endif