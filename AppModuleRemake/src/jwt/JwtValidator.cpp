#include <iostream>
#include <jwt-cpp/jwt.h>
#include "JwtConfiguration.h"

namespace Jwt {
    class JwtValidator {
        public:
            static bool validate(const std::string& token);
    };

    bool JwtValidator::validate(const std::string& token) {
        try {
        auto dtoken = jwt::decode(token);
        jwt::verify()
        .allow_algorithm(jwt::algorithm::hs256{std::string(Jwt::Configuration::Auth::SECRET_KEY)})
        .verify(dtoken);

        auto exp = dtoken.get_payload_claim("exp").as_date();
        auto currentTime = std::chrono::system_clock::now();

        if(exp < currentTime) return false;
        return true;
        } catch(std::exception& e) { 
            return false; }
    }
}