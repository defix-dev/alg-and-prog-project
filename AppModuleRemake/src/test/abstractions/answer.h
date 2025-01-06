#ifndef ANSWER_H
#define ANSWER_H

#include <iostream>

class Answer {
    public:
        Answer(bool isCorrect = false);
        bool isCorrect();
    private:
        bool m_isCorrect;
};

#endif