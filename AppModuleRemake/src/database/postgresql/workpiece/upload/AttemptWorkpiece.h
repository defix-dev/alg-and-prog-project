#ifndef ATTEMPT_WORKPIECE_H
#define ATTEMPT_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct AttemptWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& testId,
                    const std::string& userId,
                    const std::string& mark,
                    const std::string& completed
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("user_id", userId),
                        Abstraction::Field("test_id", testId),
                        Abstraction::Field("mark", mark),
                        Abstraction::Field("is_completed", completed)
                    });
                }
        };
    }
}

#endif