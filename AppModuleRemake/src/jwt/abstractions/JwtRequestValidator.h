#ifndef JWT_REQURST_VALIDATOR_H
#define JWT_REQURST_VALIDATOR_H

#include "crow.h"

namespace Jwt {
    class JwtRequestValidator {
        public:
            static bool isValid(const crow::request& req);
    };
}

#endif