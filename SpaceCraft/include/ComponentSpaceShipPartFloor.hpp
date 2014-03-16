#ifndef _COMPONENTSPACESHIPPARTFLOOR_HPP_
#define _COMPONENTSPACESHIPPARTFLOOR_HPP_

#include "ComponentSpaceShipPart.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentSpaceShipPartFloor : public ComponentSpaceShipPart
{
public:
    ComponentSpaceShipPartFloor(Object *object, ParamMap &params);
    ~ComponentSpaceShipPartFloor();

    static void *createInstance(Object *object, ParamMap &params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    static SpaceShipPartInfo mPartInfo[14];

};

};

#endif
