#ifndef _UNIVERSE_HPP_
#define _UNIVERSE_HPP_

#include "Map.hpp"


class Universe : public Map
{
public:
    Universe(ENGINE *engine);

    void init();
    bool update(float elapsedTime);

private:
};

#endif