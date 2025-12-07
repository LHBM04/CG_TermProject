#pragma once

#include "../Framework/Common.h"

class GameManager
{
public:
    static inline int currentLevel   = 0;
    static inline int maxLevel       = 7;
    static inline int deathCount     = 0;
    static inline float playTime     = 0.0f;

    static void Initialize()
    {
        currentLevel = 0;
    }

    static void NextLevel()
    {
        currentLevel++;
    }
};