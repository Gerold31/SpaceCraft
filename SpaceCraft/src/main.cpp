#include "ENGINE.hpp"
#include "Game.hpp"

#include "OGRE/OgreException.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <stdio.h>

#define ENGINE_ERROR(x) printf(x);

int main(int argc, char **argv)
{
    try{
        ENGINE *engine = new ENGINE();
        Game *map = new Game(Game::DIFFICULTY_NORMAL, engine);

        if(!engine->init(map))
        {
            ENGINE_ERROR("failed to initialize ENGINE");
            return -1;
        }

        engine->run();

        delete engine;
        engine = NULL;
    }catch(Ogre::Exception &e)
    {
        ENGINE_ERROR(e.what());
    }catch(std::exception &e)
    {
        ENGINE_ERROR(e.what());
    }catch(...)
    {
        ENGINE_ERROR("Unknown Error");
    }
    return 0;
}
