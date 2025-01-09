#ifndef COURSE_WORKPIECE_H
#define COURSE_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct CourseWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& name,
                    const std::string& description,
                    const std::string& teacherId,
                    const std::string& isDeleted
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("name", name),
                        Abstraction::Field("description", description),
                        Abstraction::Field("teacher_id", teacherId),
                        Abstraction::Field("is_deleted", isDeleted)
                    });
                }
        };
    }
}

#endif