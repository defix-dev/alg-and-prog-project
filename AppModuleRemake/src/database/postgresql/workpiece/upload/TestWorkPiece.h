#ifndef TEST_WORKPIECE_H
#define TEST_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct TestWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& name,
                    const std::string& description,
                    const std::string& courseId,
                    const std::string& isActive,
                    const std::string& isDeleted
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("name", name),
                        Abstraction::Field("description", description),
                        Abstraction::Field("course_id", courseId),
                        Abstraction::Field("is_active", isActive),
                        Abstraction::Field("is_deleted", isDeleted)
                    });
                }
        };
    }
}

#endif