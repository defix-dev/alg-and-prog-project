#ifndef SERVER_CONFIGURATION_H
#define SERVER_CONFIGURATION_H

namespace Server {
    namespace Configuration {
        struct Run
        {
            static constexpr int PORT = 3032;
        };

        namespace Permissions {
            struct Users {
                static constexpr const char* GET_USERS_LIST = "user:list:read";
                static constexpr const char* SET_FULL_NAME = "user:fullName:write";
                static constexpr const char* GET_USER_INFO = "user:data:read";
                static constexpr const char* GET_ROLE = "user:roles:read";
                static constexpr const char* SET_ROLE = "user:roles:write";
                static constexpr const char* GET_BLOCK_STATUS = "user:block:read";
                static constexpr const char* SET_BLOCK_STATUS = "user:block:write";
            };

            struct Courses {
                static constexpr const char* SET_COURSE_INFO = "course:info:write";
                static constexpr const char* GET_TESTS_LIST = "course:testList";
                static constexpr const char* GET_TEST_STATUS = "course:test:read";
                static constexpr const char* SET_TEST_STATUS = "course:test:write";
                static constexpr const char* ADD_TEST = "course:test:add";
                static constexpr const char* DELETE_TEST = "course:test:del";
                static constexpr const char* GET_USERS_LIST = "course:userList";
                static constexpr const char* ADD_USER = "course:user:add";
                static constexpr const char* DELETE_USER = "course:user:del";
                static constexpr const char* ADD_COURSE = "course:add";
                static constexpr const char* DELETE_COURSE = "course:del";
            };
        };

        struct AuthServ {
            static constexpr int PORT = 3031;
            static constexpr const char* HOST = "localhost";
            static constexpr const char* DOMAIN_NAME = "localhost:3031";
        };
    }
}


#endif
