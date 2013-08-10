#include "SpaceShipPartWall.hpp"

#include "ENGINE.hpp"

#include "SpaceShipPartFloor.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLight.h"


SpaceShipPart::SpaceShipPartInfo SpaceShipPartWall::mPartInfo[]     = {SpaceShipPartInfo(PART_WALLMOUNT, Ogre::Vector3(-WALL_SIZE_X/2, 0, 0), Ogre::Quaternion(1, 0, 0, 0), false),
                                                                       SpaceShipPartInfo(PART_WALLMOUNT, Ogre::Vector3( WALL_SIZE_X/2, 0, 0), Ogre::Quaternion(1, 0, 0, 0), false),

                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, 0, WALL_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), false),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, 0,-WALL_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), false),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, WALL_SIZE_Y, 0), Ogre::Quaternion(1, 0, 0, 0),  true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0,-WALL_SIZE_Y, 0), Ogre::Quaternion(1, 0, 0, 0),  true),

                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

SpaceShipPartWall::SpaceShipPartWall(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine, Ogre::String type)
    : SpaceShipPart(PART_WALL, true, pos, ori, parent, name, type, engine)
{
    if(type == "SC_SpaceShipPartWall")
    {
        mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "SpaceShip/Part_Wall.mesh");
        mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
        mNode->attachObject(mEntity);
    }else
        mEntity = NULL;

    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}

SpaceShipPartWall::SpaceShipPartWall(SpaceShipPart *old, Ogre::String name, Ogre::String type)
    : SpaceShipPart(old, name, type)
{
}

bool SpaceShipPartWall::update(float elapsedTime)
{
    return true;
}
