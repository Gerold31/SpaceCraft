#ifndef _COMPONENTSPACESHIPPARTCEILMOUNT_HPP_
#define _COMPONENTSPACESHIPPARTCEILMOUNT_HPP_

#include "ComponentSpaceShipPart.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentSpaceShipPartCeilMount : public ComponentSpaceShipPart
{
public:
    ComponentSpaceShipPartCeilMount(Object *object, ParamMap &params);
    ~ComponentSpaceShipPartCeilMount();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    static SpaceShipPartInfo mPartInfo[1];

};

};

#endif
