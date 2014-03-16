#include "ComponentSpaceShipPartFloorMount.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace SpaceCraft;

TypeInfo *ComponentSpaceShipPartFloorMount::mType = new TypeInfo("ComponentSpaceShipPartFloorMount", &ComponentSpaceShipPartFloorMount::createInstance);

ComponentSpaceShipPart::SpaceShipPartInfo ComponentSpaceShipPartFloorMount::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, -FLOOR_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

ComponentSpaceShipPartFloorMount::ComponentSpaceShipPartFloorMount(Object *object, ParamMap &params) :
    ComponentSpaceShipPart(object, params, mType, PART_FLOORMOUNT)
{
	LOG_IN("component");
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<ComponentSpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
	LOG_OUT("component");
}

ComponentSpaceShipPartFloorMount::~ComponentSpaceShipPartFloorMount()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentSpaceShipPartFloorMount::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentSpaceShipPartFloorMount(object, params);
}

void ComponentSpaceShipPartFloorMount::init()
{
	LOG_IN("component");
	LOG_OUT("component");
}
    
void ComponentSpaceShipPartFloorMount::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentSpaceShipPartFloorMount::receiveMessage(Message *message)
{
	LOG_IN_MSG;
	LOG_OUT_MSG;
}
