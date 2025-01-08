#include "abstractions/JwtRequestParser.h"

namespace Jwt {
    std::string JwtRequestParser::parse(const crow::request& req) {
        return req.get_header_value("Authorization").substr(7);
    }
}