#include "ComponentSpaceShipPartWallMount.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace SpaceCraft;

TypeInfo *ComponentSpaceShipPartWallMount::mType = new TypeInfo("ComponentSpaceShipPartWallMount", &ComponentSpaceShipPartWallMount::createInstance);

ComponentSpaceShipPart::SpaceShipPartInfo ComponentSpaceShipPartWallMount::mPartInfo[] = {SpaceShipPartInfo(PART_WALL, Ogre::Vector3(-WALL_SIZE_X/2, 0, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

ComponentSpaceShipPartWallMount::ComponentSpaceShipPartWallMount(Object *object, ParamMap &params) :
    ComponentSpaceShipPart(object, params, mType, PART_WALLMOUNT)
{
	LOG_IN("component");
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<ComponentSpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
	LOG_OUT("component");
}

ComponentSpaceShipPartWallMount::~ComponentSpaceShipPartWallMount()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentSpaceShipPartWallMount::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentSpaceShipPartWallMount(object, params);
}

bool ComponentSpaceShipPartWallMount::init()
{
	LOG_IN("component");
    mReady = true;
	LOG_OUT("component");
    return true;
}
    
void ComponentSpaceShipPartWallMount::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentSpaceShipPartWallMount::_receiveMessage(Message *message)
{
	LOG_IN_MSG;
	LOG_OUT_MSG;
}
