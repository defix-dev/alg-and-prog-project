#include <iostream>
#include <list>
#include <answer.h>

class Quest {
    public:
        Quest(const std::list<Answer*>& answers);
        int getMaxCorrectAnswers();
        int getCurrentCorrectAnswers();
        void applyAnswers(const std::list<int>& answerIds);
        bool isCorrect();
        ~Quest();
    private:
        std::list<Answer*> m_answers;
        int m_correctAnswers = 0;
        int m_maxCorrectAnswers = -1;
};