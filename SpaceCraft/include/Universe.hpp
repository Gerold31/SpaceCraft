#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#include "Map.hpp"

class GravityObject;

class Universe : public Map
{
public:
    Universe(ENGINE *engine);

    void init();
    bool update(float elapsedTime);

    void addGravityObject(GravityObject *obj) {mGravityObjects.push_back(obj);}

private:
    std::vector<GravityObject *> mGravityObjects;
};

#endif
