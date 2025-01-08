#ifndef JWT_VALIDATOR_H
#define JWT_VALIDATOR_H

#include <iostream>

namespace Jwt {
    class JwtValidator {
        public:
            static bool validate(const std::string& token);
    };
}

#endif