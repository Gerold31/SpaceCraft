#include "Map.hpp"

#include "ENGINE.hpp"
#include "Entity.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"

Map::Map(ENGINE *engine)
{
    mEngine = engine;
}

void Map::init()
{
}

bool Map::update(float elapsedTime)
{
    for(std::vector<Entity *>::iterator i=mEntitiesToRegister.begin(); i!=mEntitiesToRegister.end(); ++i)
    {
        mEntities.push_back(*i);
    }
    mEntitiesToRegister.clear();
    for(std::vector<Entity *>::iterator i=mEntities.begin(); i!=mEntities.end(); ++i)
    {
        if(!((*i)->update(elapsedTime)))
            return false;
    }
    return true;
}

void Map::registerEntity(Entity *ent)
{
    mEntitiesToRegister.push_back(ent);
}