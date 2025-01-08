#ifndef SERVER_CONFIGURATION_H
#define SERVER_CONFIGURATION_H

namespace Server {
    namespace Configuration {
        struct Run
        {
            static constexpr int PORT = 3032;
        };

        struct Permissions {
            static constexpr const char* GET_USERS_LIST = "user:list:read";
            static constexpr const char* SET_FULL_NAME = "user:fullName:write";
            static constexpr const char* GET_USER_INFO = "user:data:read";
            static constexpr const char* GET_ROLES = "user:roles:read";
            static constexpr const char* SET_ROLES = "user:roles:write";
            static constexpr const char* IS_BLOCKED = "user:block:read";
            static constexpr const char* SET_BLOCK_STATUS = "user:block:write";
        };

        struct AuthServ {
            static constexpr int PORT = 3031;
            static constexpr const char* HOST = "localhost";
            static constexpr const char* DOMAIN_NAME = "localhost:3031";
        };
    }
}


#endif
