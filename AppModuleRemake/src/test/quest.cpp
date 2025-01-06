#include <iostream>
#include "abstractions/quest.h"

Quest::Quest(const std::list<Answer*>& answers) {
    m_answers = answers;
}

int Quest::getMaxCorrectAnswers() {
    if(m_maxCorrectAnswers != -1) return m_maxCorrectAnswers;
    int max = 0;
    for(auto& answer : m_answers) {
        max += answer->isCorrect() ? 1 : 0;
    }
    m_maxCorrectAnswers = max;
    return max;
}

int Quest::getCurrentCorrectAnswers() {
    return m_correctAnswers;
}

void Quest::applyAnswers(const std::list<int>& answerIds) {
    int correctAnswers = 0;
    std::list<Answer*>::iterator it = m_answers.begin();
    for(auto& id : answerIds) {
        std::advance(it, id);
        correctAnswers += (*it)->isCorrect() ? 1 : 0;
    }
    m_correctAnswers = correctAnswers;
}

bool Quest::isCorrect() {
    return m_correctAnswers == getMaxCorrectAnswers();
}

Quest::~Quest() {
    for(auto ptr : m_answers) {
        delete ptr;
    }
    m_answers.clear();
}