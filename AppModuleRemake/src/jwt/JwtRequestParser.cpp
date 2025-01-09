#include "abstractions/JwtRequestParser.h"
#include "abstractions/JwtAdapter.h"

namespace Jwt {
    AccessTokenBody JwtRequestParser::parse(const crow::request& req) {
        return JwtAdapter::getAccessTokenBody(req.get_header_value("Authorization").substr(7));
    }
}