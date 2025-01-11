#ifndef QUEST_TEST_WORKPIECE_H
#define QUEST_TEST_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct QuestTestWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& questId,
                    const std::string& questVersion,
                    const std::string& testId
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("quest_id", questId),
                        Abstraction::Field("quest_version", questVersion),
                        Abstraction::Field("test_id", testId)
                    });
                }
        };
    }
}

#endif