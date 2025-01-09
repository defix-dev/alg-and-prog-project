#ifndef USER_COURSE_WORKPIECE_H
#define USER_COURSE_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct UserCourseWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& userId,
                    const std::string& courseId
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("user_id", userId),
                        Abstraction::Field("course_id", courseId)
                    });
                }
        };
    }
}

#endif