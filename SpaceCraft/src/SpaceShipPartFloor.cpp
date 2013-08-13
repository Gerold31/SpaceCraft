#include "SpaceShipPartFloor.hpp"

#include "ENGINE.hpp"

#include "SpaceShipPartWall.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPartInfo SpaceShipPartFloor::mPartInfo[]    = {SpaceShipPartInfo(PART_FLOORMOUNT,Ogre::Vector3(0, FLOOR_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), false),
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

Ogre::Entity *SpaceShipPartFloor::mStaticEntity = NULL;

std::string SpaceShipPartFloor::mType = "SC_SpaceShipPartFloor";

SpaceShipPartFloor::SpaceShipPartFloor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_FLOOR, true, pos, ori, parent, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "SpaceShip/Part_Floor.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    commonConstructor();
}

SpaceShipPartFloor::SpaceShipPartFloor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_FLOOR, true, pos, ori, parent, staticGeometry, name, mType, engine)
{
    if(!mStaticEntity)
    {
        mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "SpaceShip/Part_Floor.mesh");
        mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    }
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);

    commonConstructor();
}

void SpaceShipPartFloor::commonConstructor()
{
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}

SpaceShipPartFloor::SpaceShipPartFloor(SpaceShipPart *old, Ogre::String name)
    : SpaceShipPart(old, name)
{
}

bool SpaceShipPartFloor::update(float elapsedTime)
{
    return true;
}
