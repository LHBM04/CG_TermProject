#pragma once

#include "../Framework/Common.h"

class GameManager
{
public:
    static int currentLevel;

    static void Initialize()
    {
        currentLevel = 1;
    }

    static void NextLevel()
    {
        currentLevel++;
    }
};

inline int GameManager::currentLevel = 1;