#ifndef _COMPONENTSPACESHIPPARTWALL_HPP_
#define _COMPONENTSPACESHIPPARTWALL_HPP_

#include "ComponentSpaceShipPart.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentSpaceShipPartWall : public ComponentSpaceShipPart
{
public:
    ComponentSpaceShipPartWall(Object *object, ParamMap &params);
    ~ComponentSpaceShipPartWall();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    static SpaceShipPartInfo mPartInfo[14];

};

};

#endif