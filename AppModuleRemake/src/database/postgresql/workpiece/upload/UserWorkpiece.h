#ifndef USER_WORKPIECE_H
#define USER_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct UserWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& email,
                    const std::string& fullName,
                    const std::string& id
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("email", email),
                        Abstraction::Field("id", id),
                        Abstraction::Field("full_name", fullName)
                    });
                }
        };
    }
}

#endif