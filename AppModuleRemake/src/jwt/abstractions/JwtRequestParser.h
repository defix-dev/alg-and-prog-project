#ifndef JWT_REQUEST_PARSER_H
#define JWT_REQUEST_PARSER_H

#include "crow.h"
#include "AccessTokenBody.h"

namespace Jwt {
    class JwtRequestParser {
        public:
            static AccessTokenBody parse(const crow::request& req);
    };
}

#endif