#include "SpaceShipPartDoor.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLight.h"

#define OPEN_Z (1.0)
#define CLOSE_Z (0.0)

#define DOOR_SPEED (2.0)

SpaceShipPartDoor::SpaceShipPartDoor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPartWall(pos, ori, parent, name, engine, "SC_SpaceShipPartDoor")
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "DoorFrame.mesh");
    mDoor   = engine->getSceneMgr()->createEntity(name + "DoorMesh", "Door.mesh");
    mNode->attachObject(mEntity);
    mDoorNode = mNode->createChildSceneNode(name + "DoorNode");
    mDoorNode->attachObject(mDoor);
    mLocked = mOpened = false;
}

SpaceShipPartDoor::SpaceShipPartDoor(SpaceShipPart *old, Ogre::String name)
    : SpaceShipPartWall(old, name, "SC_SpaceShipPartDoor")
{
    if(mEntity)
        mEngine->getSceneMgr()->destroyEntity(mEntity);
    mEntity = mEngine->getSceneMgr()->createEntity(name + "Mesh", "DoorFrame.mesh");
    mDoor   = mEngine->getSceneMgr()->createEntity(name + "DoorMesh", "Door.mesh");
    mNode->attachObject(mEntity);
    mDoorNode = mNode->createChildSceneNode(name + "DoorNode");
    mDoorNode->attachObject(mDoor);
    mLocked = mOpened = false;
}

bool SpaceShipPartDoor::update(float elapsedTime)
{
    Ogre::Vector3 pos = mDoorNode->getPosition();
    if(mOpened && pos.z != OPEN_Z)
    {
        float movement = DOOR_SPEED*elapsedTime;
        if(pos.z + movement >= OPEN_Z)
            mDoorNode->setPosition(pos.x, pos.y, OPEN_Z);
        else
            mDoorNode->setPosition(pos.x, pos.y, pos.z + movement);
    }else if(!mOpened && pos.z != CLOSE_Z)
    {
        float movement = -DOOR_SPEED*elapsedTime;
        if(pos.z + movement <= CLOSE_Z)
            mDoorNode->setPosition(pos.x, pos.y, CLOSE_Z);
        else
            mDoorNode->setPosition(pos.x, pos.y, pos.z + movement);
    }
    return true;
}

void SpaceShipPartDoor::open(bool open)
{
    if(!mLocked)
        mOpened = open;
}

void SpaceShipPartDoor::lock(bool lock)
{
    mLocked = lock;
}