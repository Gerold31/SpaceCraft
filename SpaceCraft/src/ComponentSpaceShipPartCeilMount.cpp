#include "ComponentSpaceShipPartCeilMount.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace SpaceCraft;

TypeInfo *ComponentSpaceShipPartCeilMount::mType = new TypeInfo("ComponentSpaceShipPartCeilMount", &ComponentSpaceShipPartCeilMount::createInstance);

ComponentSpaceShipPart::SpaceShipPartInfo ComponentSpaceShipPartCeilMount::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, FLOOR_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

ComponentSpaceShipPartCeilMount::ComponentSpaceShipPartCeilMount(Object *object, ParamMap &params) :
    ComponentSpaceShipPart(object, params, mType, PART_CEILMOUNT)
{
	LOG_IN("component");
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<ComponentSpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
	LOG_OUT("component");
}

ComponentSpaceShipPartCeilMount::~ComponentSpaceShipPartCeilMount()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentSpaceShipPartCeilMount::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentSpaceShipPartCeilMount(object, params);
}

void ComponentSpaceShipPartCeilMount::init()
{
	LOG_IN("component");
	LOG_OUT("component");
}
    
void ComponentSpaceShipPartCeilMount::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentSpaceShipPartCeilMount::receiveMessage(Message *message)
{
	LOG_IN_MSG;
	LOG_OUT_MSG;
}
