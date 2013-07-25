#ifndef _MAP_HPP_
#define _MAP_HPP_

class Entity;
class ENGINE;

#include <vector>

class Map
{
public:
    Map(ENGINE *engine);
    
    virtual void init();
    virtual bool update(float elapsedTime);

    void registerEntity(Entity *ent);

    void destroyEntity(Entity *ent);

protected:
    std::vector<Entity *> mEntities;
    std::vector<Entity *> mEntitiesToRegister;
    ENGINE *mEngine;
};

#endif