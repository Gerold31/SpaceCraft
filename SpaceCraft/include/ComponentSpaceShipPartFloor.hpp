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
