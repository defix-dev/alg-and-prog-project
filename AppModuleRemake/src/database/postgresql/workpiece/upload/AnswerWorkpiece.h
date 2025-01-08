#ifndef ANSWER_WORKPIECE_H
#define ANSWER_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct AnswerWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& name,
                    const std::string& questId,
                    const std::string& isCorrect
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("name", name),
                        Abstraction::Field("quest_id", questId),
                        Abstraction::Field("is_correct", isCorrect)
                    });
                }
        };
    }
}

#endif