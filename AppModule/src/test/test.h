#include <iostream>
#include <list>
#include <quest.h>

class Test {
    public:
        Test(const std::list<Quest*>& quests);
        int getCurrentPoints();
        int getCurrentCorrectQuests();
        ~Test();
    private:
        std::list<Quest*> m_quests;
};