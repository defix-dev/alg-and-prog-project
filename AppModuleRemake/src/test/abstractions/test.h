#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <list>
#include "quest.h"

class Test {
    public:
        Test(const std::list<Quest>& quests);
        int getCurrentPoints();
        int getCurrentCorrectQuests();
    private:
        std::list<Quest> m_quests;
};

#endif