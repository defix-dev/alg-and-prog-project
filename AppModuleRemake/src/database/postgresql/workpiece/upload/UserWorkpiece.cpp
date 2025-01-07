#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct UserWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& email,
                    const std::string& fullName
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("email", email),
                        Abstraction::Field("full_name", fullName)
                    });
                }
        };
    }
}