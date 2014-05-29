#include "ComponentSpaceShipPartWall.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace SpaceCraft;

TypeInfo *ComponentSpaceShipPartWall::mType = new TypeInfo("ComponentSpaceShipPartWall", &ComponentSpaceShipPartWall::createInstance);

ComponentSpaceShipPart::SpaceShipPartInfo ComponentSpaceShipPartWall::mPartInfo[] =
   {SpaceShipPartInfo(PART_WALLMOUNT, Ogre::Vector3(-WALL_SIZE_X/2, 0, 0), Ogre::Quaternion(1, 0, 0, 0), false),
    SpaceShipPartInfo(PART_WALLMOUNT, Ogre::Vector3( WALL_SIZE_X/2, 0, 0), Ogre::Quaternion(1, 0, 0, 0), false),

    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, 0, WALL_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), false),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, 0,-WALL_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), false),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, WALL_SIZE_Y, 0), Ogre::Quaternion(1, 0, 0, 0),  true),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0,-WALL_SIZE_Y, 0), Ogre::Quaternion(1, 0, 0, 0),  true),

    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
    SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),

    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3( WALL_SIZE_Z/2, 0, WALL_SIZE_Z/2), Ogre::Quaternion(sqrt(0.5), 0, -sqrt(0.5), 0), false),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3( WALL_SIZE_Z/2, 0,-WALL_SIZE_Z/2), Ogre::Quaternion(sqrt(0.5), 0, -sqrt(0.5), 0), false),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3( WALL_SIZE_Z/2, 0, WALL_SIZE_Z/2), Ogre::Quaternion(sqrt(0.5), 0,  sqrt(0.5), 0), false),
    SpaceShipPartInfo(PART_WALL,      Ogre::Vector3( WALL_SIZE_Z/2, 0,-WALL_SIZE_Z/2), Ogre::Quaternion(sqrt(0.5), 0,  sqrt(0.5), 0), false)};

ComponentSpaceShipPartWall::ComponentSpaceShipPartWall(Object *object, ParamMap &params) :
    ComponentSpaceShipPart(object, params, mType, PART_WALL)
{
	LOG_IN("component");
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<ComponentSpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
	LOG_OUT("component");
}

ComponentSpaceShipPartWall::~ComponentSpaceShipPartWall()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentSpaceShipPartWall::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentSpaceShipPartWall(object, params);
}

bool ComponentSpaceShipPartWall::init()
{
	LOG_IN("component");
    mReady = true;
	LOG_OUT("component");
    return true;
}
    
void ComponentSpaceShipPartWall::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentSpaceShipPartWall::_receiveMessage(Message *message)
{
	LOG_IN_MSG;
	LOG_OUT_MSG;
}
