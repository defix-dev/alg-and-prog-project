#ifndef JWT_CONFIGURATION_H
#define JWT_CONFIGURATION_H

namespace Jwt {
    namespace Configuration {
        struct Auth
        {
            static constexpr const char* SECRET_KEY = "sigma_squad_key123";
        };
    }
}

#endif