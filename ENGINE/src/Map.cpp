#include "Map.hpp"

#include "ENGINE.hpp"
#include "Entity.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"

Map::Map(ENGINE *engine)
{
    mEngine = engine;
}

Map::~Map()
{
    for(std::vector<Entity *>::iterator i=mEntitiesToRegister.begin(); i!=mEntitiesToRegister.end(); ++i)
    {
        if(*i)
            delete *i;
    }
    mEntitiesToRegister.clear();
    for(std::vector<Entity *>::iterator i=mEntities.begin(); i!=mEntities.end(); ++i)
    {
        if(*i)
            delete *i;
    }
    mEntities.clear();
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
    for(std::vector<Entity *>::iterator i=mEntities.begin(); i!=mEntities.end();)
    {
        if(*i)
        {
            if(!((*i)->update(elapsedTime)))
                return false;
            ++i;
        }else
        {
            i = mEntities.erase(i);
        }
    }
    return true;
}

void Map::registerEntity(Entity *ent)
{
    mEntitiesToRegister.push_back(ent);
}

int Map::getNumberEntities()
{
    return mEntities.size(); 
}

Entity *Map::getEntity(int i)
{
    return mEntities.at(i);
}

void Map::destroyEntity(Entity *ent)
{
    for(std::vector<Entity *>::iterator i=mEntities.begin(); i!=mEntities.end(); ++i)
    {
        if(*i == ent)
        {
            delete ent;
            *i = NULL;
            return;
        }
    }
}