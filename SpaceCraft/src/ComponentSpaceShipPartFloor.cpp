#include "ComponentSpaceShipPartFloor.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace SpaceCraft;

TypeInfo *ComponentSpaceShipPartFloor::mType = new TypeInfo("ComponentSpaceShipPartFloor", &ComponentSpaceShipPartFloor::createInstance);

ComponentSpaceShipPart::SpaceShipPartInfo ComponentSpaceShipPartFloor::mPartInfo[] =
   {SpaceShipPartInfo(PART_FLOORMOUNT,Ogre::Vector3(0, FLOOR_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), false),
    SpaceShipPartInfo(PART_CEILMOUNT, Ogre::Vector3(0,-FLOOR_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), false),

    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X, 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), true),
    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0, FLOOR_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), true),
    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X, 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), true),
    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0,-FLOOR_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), true),

    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0)                 , true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0, -sqrt(0.5), 0), true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(0, 0, 1, 0)                 , true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0,  sqrt(0.5), 0), true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0)                 , true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0, -sqrt(0.5), 0), true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(0, 0, 1, 0)                 , true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0,  sqrt(0.5), 0), true)};

ComponentSpaceShipPartFloor::ComponentSpaceShipPartFloor(Object *object, ParamMap &params) :
    ComponentSpaceShipPart(object, params, mType, PART_FLOOR)
{
	LOG_IN("component");
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<ComponentSpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
	LOG_OUT("component");
}

ComponentSpaceShipPartFloor::~ComponentSpaceShipPartFloor()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentSpaceShipPartFloor::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentSpaceShipPartFloor(object, params);
}

bool ComponentSpaceShipPartFloor::init()
{
	LOG_IN("component");
    mReady = true;
	LOG_OUT("component");
    return true;
}
    
void ComponentSpaceShipPartFloor::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentSpaceShipPartFloor::_receiveMessage(Message *message)
{
	LOG_IN_MSG;
	LOG_OUT_MSG;
}
