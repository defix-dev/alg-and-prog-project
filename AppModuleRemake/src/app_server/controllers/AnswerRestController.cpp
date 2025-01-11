#include "abstractions/AnswerRestController.h"
#include "Authentication.h"
#include "Server.h"
#include "Database.h"
#include "Jwt.h"

namespace Server {
    void AnswerRestController<pqxx::connection>::initialize(const std::shared_ptr<crow::SimpleApp>& app) {
        
    }
}