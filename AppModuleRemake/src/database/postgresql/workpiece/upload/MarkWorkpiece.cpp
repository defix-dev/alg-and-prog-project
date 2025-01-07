#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct MarkWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& mark,
                    const std::string& userId,
                    const std::string& testId
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("mark", mark),
                        Abstraction::Field("user_id", userId),
                        Abstraction::Field("test_id", testId)
                    });
                }
        };
    }
}