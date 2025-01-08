#ifndef JWT_REQUEST_PARSER_H
#define JWT_REQUEST_PARSER_H

#include "crow.h"

namespace Jwt {
    class JwtRequestParser {
        public:
            static std::string parse(const crow::request& req);
    };
}

#endif