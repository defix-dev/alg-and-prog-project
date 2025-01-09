#ifndef ANSWER_WORKPIECE_H
#define ANSWER_WORKPIECE_H

#include "../../../abstractions/Data.h"

namespace Database {
    namespace Postgresql {
        struct AnswerWorkpiece {
            public:
                static Abstraction::Data get(
                    const std::string& questId,
                    const std::string& questVersion,
                    const std::string& attemptId,
                    const std::string& answer
                ) {
                    return Abstraction::Data({
                        Abstraction::Field("attempt_id", attemptId),
                        Abstraction::Field("question_id", questId),
                        Abstraction::Field("question_version", questVersion),
                        Abstraction::Field("answer", answer)
                    });
                }
        };
    }
}

#endif