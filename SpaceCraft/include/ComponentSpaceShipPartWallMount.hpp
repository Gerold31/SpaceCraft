#ifndef _COMPONENTSPACESHIPPARTWALLMOUNT_HPP_
#define _COMPONENTSPACESHIPPARTWALLMOUNT_HPP_

#include "ComponentSpaceShipPart.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentSpaceShipPartWallMount : public ComponentSpaceShipPart
{
public:
    ComponentSpaceShipPartWallMount(Object *object, ParamMap &params);
    ~ComponentSpaceShipPartWallMount();

    static void *createInstance(Object *object, ParamMap &params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    static SpaceShipPartInfo mPartInfo[1];

};

};

#endif
