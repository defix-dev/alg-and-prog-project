#include "abstractions/JwtRequestValidator.h"
#include "abstractions/JwtValidator.h"

namespace Jwt {
    bool JwtRequestValidator::isValid(const crow::request& req) {
        auto authHeader = req.get_header_value("Authorization");

        if(authHeader.empty() || authHeader.substr(0, 7) != "Bearer ") {
            std::cout << "EMPTY:()";
            return false;
        }

        return JwtValidator::validate(authHeader.substr(7));
    }
}