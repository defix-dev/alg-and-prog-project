#ifndef QUEST_WORKPIECE_H
#define QUEST_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct QuestWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& name,
                    const std::string& text,
                    const std::string& authorId,
                    const std::string& answers,
                    const std::string& correctAsnwer,
                    const std::string& version,
                    const std::string& isDeleted
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("name", name),
                        Abstraction::Field("text", text),
                        Abstraction::Field("author_id", authorId),
                        Abstraction::Field("answers", answers),
                        Abstraction::Field("correct_answer", correctAsnwer),
                        Abstraction::Field("version", version),
                        Abstraction::Field("is_deleted", isDeleted)
                    });
                }
        };
    }
}

#endif