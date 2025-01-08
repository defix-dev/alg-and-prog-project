#ifndef JWT_ADAPTER_H
#define JWT_ADAPTER_H

#include "AccessTokenBody.h"

namespace Jwt {
    class JwtAdapter {
        public:
            static AccessTokenBody getAccessTokenBody(const std::string& token);
    };
}

#endif