#include<iostream>
#include <answer.h>

Answer::Answer(bool isCorrect) {
    m_isCorrect = isCorrect;
}

bool Answer::isCorrect() {
    return m_isCorrect;
}