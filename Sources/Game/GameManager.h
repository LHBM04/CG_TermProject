#pragma once

#include "../Framework/Common.h"

class GameManager
{
public:
    static int currentLevel;
    static int maxLevel;

    static void Initialize()
    {
        currentLevel = 0;
    }

    static void NextLevel()
    {
        currentLevel++;
    }
};

inline int GameManager::currentLevel = 0;
inline int GameManager::maxLevel     = 2;