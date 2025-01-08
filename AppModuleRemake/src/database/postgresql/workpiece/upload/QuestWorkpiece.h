#ifndef QUEST_WORKPIECE_H
#define QUEST_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct QuestWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& name,
                    const std::string& testId
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("name", name),
                        Abstraction::Field("test_id", testId)
                    });
                }
        };
    }
}

#endif