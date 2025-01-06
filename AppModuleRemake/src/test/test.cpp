#include <iostream>
#include "abstractions/test.h"

Test::Test(const std::list<Quest*>& quests) {
    m_quests = quests;
}

int Test::getCurrentPoints() {
    int points = 0;
    for(auto& quest : m_quests) {
        points += quest->getCurrentCorrectAnswers();
    }
    return points;
}

int Test::getCurrentCorrectQuests() {
    int correctAnswers = 0;
    for(auto& quest : m_quests) {
        correctAnswers += quest->isCorrect() ? 1 : 0;
    }
    return correctAnswers;
}

Test::~Test() {
    for(auto ptr : m_quests) {
        delete ptr;
    }
    m_quests.clear();
}