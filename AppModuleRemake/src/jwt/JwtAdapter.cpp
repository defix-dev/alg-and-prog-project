#include<iostream>
#include<jwt-cpp/jwt.h>

namespace Jwt {
    class JwtAdapter {
        public:
            static std::vector<std::string> getPermissionsFromAccessToken(const std::string& token);
    };

    std::vector<std::string> JwtAdapter::getPermissionsFromAccessToken(const std::string& token) {
        auto dtoken = jwt::decode(token);

        auto dpermissions = dtoken.get_payload_claim("permissions").as_array();

        std::vector<std::string> permissions{};
        for(const auto& permission : dpermissions) {
            permissions.push_back(permission.to_str());
        }

        return permissions;
    }
}