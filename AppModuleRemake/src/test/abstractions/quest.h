#ifndef QUEST_H
#define QUEST_H

#include <iostream>
#include <list>
#include <vector>
#include "answer.h"

class Quest {
    public:
        Quest(const std::list<Answer>& answers);
        int getMaxCorrectAnswers();
        int getCurrentCorrectAnswers();
        void applyAnswers(const std::vector<int>& answerIds);
        bool isCorrect();
    private:
        std::list<Answer> m_answers;
        int m_correctAnswers = 0;
        int m_maxCorrectAnswers = -1;
};

#endif