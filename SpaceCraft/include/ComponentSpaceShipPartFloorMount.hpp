#ifndef _COMPONENTSPACESHIPPARTFLOORMOUNT_HPP_
#define _COMPONENTSPACESHIPPARTFLOORMOUNT_HPP_

#include "ComponentSpaceShipPart.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentSpaceShipPartFloorMount : public ComponentSpaceShipPart
{
public:
    ComponentSpaceShipPartFloorMount(Object *object, ParamMap &params);
    ~ComponentSpaceShipPartFloorMount();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    static SpaceShipPartInfo mPartInfo[1];

};

};

#endif
