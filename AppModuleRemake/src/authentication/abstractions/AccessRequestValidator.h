#ifndef ACCESS_REQUEST_VALIDATOR_H
#define ACCESS_REQUEST_VALIDATOR_H

#include <pqxx/pqxx>
#include "ValidateInfo.h"
#include "crow.h"

namespace Auth {
    class AccessRequestValidator {
        public:
            static ValidateInfo validate(const crow::request& req, const std::string& permission = "");
            static bool matchUserId(int id, const crow::request& req);

            template<typename DB>
            static bool matchCourseIdOwner(int id, const crow::request& req, const std::shared_ptr<DB>& db);

            template<typename DB>
            static bool matchCourseIdSubscriber(int id, const crow::request& req, const std::shared_ptr<DB>& db);

            template<typename DB>
            static bool matchAttemptExist(int id, int version, const crow::request& req, const std::shared_ptr<DB>& db);

            template<typename DB>
            static bool matchQuestIdOwner(int id, int version, const crow::request& req, const std::shared_ptr<DB>& db);

            template<typename DB>
            static bool matchTestIdSubscriber(int id, const crow::request& req, const std::shared_ptr<DB>& db);

            template<typename DB>
            static ValidateInfo validateWithBlockHandle(const crow::request& req, const std::shared_ptr<DB>& db, const std::string& permission = "");
    };

    template<>
    bool AccessRequestValidator::matchCourseIdOwner<pqxx::connection>(int id, const crow::request& req, const std::shared_ptr<pqxx::connection>& db);

    template<>
    bool AccessRequestValidator::matchCourseIdSubscriber<pqxx::connection>(int id, const crow::request& req, const std::shared_ptr<pqxx::connection>& db);

    template<>
    bool AccessRequestValidator::matchAttemptExist<pqxx::connection>(int id, int version, const crow::request& req, const std::shared_ptr<pqxx::connection>& db);

    template<>
    bool AccessRequestValidator::matchQuestIdOwner<pqxx::connection>(int id, int version, const crow::request& req, const std::shared_ptr<pqxx::connection>& db);

    template<>
    bool AccessRequestValidator::matchTestIdSubscriber<pqxx::connection>(int id, const crow::request& req, const std::shared_ptr<pqxx::connection>& db);

    template<>
    ValidateInfo AccessRequestValidator::validateWithBlockHandle<pqxx::connection>(const crow::request& req, const std::shared_ptr<pqxx::connection>& db, const std::string& permission);
}

#endif