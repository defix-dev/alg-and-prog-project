#include<iostream>
#include<jwt-cpp/jwt.h>
#include "abstractions/JwtAdapter.h"

namespace Jwt {
    AccessTokenBody JwtAdapter::getAccessTokenBody(const std::string& token) {
        auto dtoken = jwt::decode(token);

        auto dpermissions = dtoken.get_payload_claim("permissions").as_array();
        auto did = dtoken.get_payload_claim("id").as_integer();

        std::vector<std::string> permissions{};
        for(const auto& permission : dpermissions) {
            permissions.push_back(permission.to_str());
        }

        return AccessTokenBody(did, permissions);
    }
}