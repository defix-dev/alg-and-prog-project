#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct CourseWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& name,
                    const std::string& userId
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("name", name),
                        Abstraction::Field("user_id", userId)
                    });
                }
        };
    }
}