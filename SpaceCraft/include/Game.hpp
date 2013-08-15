#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "Map.hpp"

class Game : public Map
{
public:
    enum Difficulty
    {
        DIFFICULTY_EASY,
        DIFFICULTY_NORMAL,
        DIFFICULTY_HARD,
        DIFFICULTY_SUIZIDAL
    };
    Game(Difficulty difficulty, ENGINE *engine);
    ~Game();
    
    virtual void init();
    virtual bool update(float elapsedTime);

protected:
    Difficulty mDifficulty;
};

#endif
