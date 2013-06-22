#include "Universe.hpp"

Universe::Universe(ENGINE *engine)
    : Map(engine)
{
}

void Universe::init()
{
}


bool Universe::update(float elapsedTime)
{
    
    return Map::update(elapsedTime);
}