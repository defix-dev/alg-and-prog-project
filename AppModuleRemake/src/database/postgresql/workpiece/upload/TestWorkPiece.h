#ifndef TEST_WORKPIECE_H
#define TEST_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct TestWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& name,
                    const std::string& courseId
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("name", name),
                        Abstraction::Field("course_id", courseId)
                    });
                }
        };
    }
}

#endif