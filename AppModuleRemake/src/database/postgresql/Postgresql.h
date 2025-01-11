#ifndef DATABASE_CONFIGURATION_H
#define DATABASE_CONFIGURATION_H

namespace Database {
    namespace Configuration {
        namespace Postgresql {
            struct Run
            {
                static constexpr int PORT = 8080;
                static constexpr const char* USER = "postgres";
                static constexpr const char* PASS = "root";
                static constexpr const char* DB_NAME = "sigma-squad-postgresql";
            };

            struct Tables {
                static constexpr const char* USERS = "users";
                static constexpr const char* COURSES = "courses";
                static constexpr const char* TESTS = "tests";
                static constexpr const char* QUESTS = "questions";
                static constexpr const char* ANSWERS = "answers";
                static constexpr const char* ATTEMPTS = "attempts";
                static constexpr const char* USERS_COURSES = "users_courses";
                static constexpr const char* QUESTS_TESTS = "quests_tests";
            };
        }
    }
}

#endif