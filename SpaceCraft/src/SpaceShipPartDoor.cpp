#include "SpaceShipPartDoor.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLight.h"

#define OPEN_Z (1.0)
#define CLOSE_Z (0.0)

#define DOOR_SPEED (2.0)

std::string SpaceShipPartDoor::mType = "SC_SpaceShipPartDoor";

SpaceShipPartDoor::SpaceShipPartDoor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPartWall(pos, ori, parent, name, engine, mType)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "DoorFrame.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
    
    commonConstructor();
}

SpaceShipPartDoor::SpaceShipPartDoor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    : SpaceShipPartWall(pos, ori, parent, staticGeometry, name, engine, mType)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "DoorFrame.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));    
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);

    commonConstructor();
}

void SpaceShipPartDoor::commonConstructor()
{
    mDoor   = mEngine->getSceneMgr()->createEntity(mName + "DoorMesh", "Door.mesh");
    mDoor->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mDoorNode = mNode->createChildSceneNode(mName + "DoorNode");
    mDoorNode->attachObject(mDoor);

    mLocked = mOpened = false;
    
    mControl = NULL;
}

SpaceShipPartDoor::SpaceShipPartDoor(SpaceShipPart *old, Ogre::String name)
    : SpaceShipPartWall(old, name, mType)
{
    if(mEntity)
        mEngine->getSceneMgr()->destroyEntity(mEntity);
    mEntity = mEngine->getSceneMgr()->createEntity(name + "Mesh", "DoorFrame.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mDoor   = mEngine->getSceneMgr()->createEntity(name + "DoorMesh", "Door.mesh");
    mDoor->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
    mDoorNode = mNode->createChildSceneNode(name + "DoorNode");
    mDoorNode->attachObject(mDoor);
    mLocked = mOpened = false;
}

SpaceShipPartDoor::~SpaceShipPartDoor()
{
    mEngine->getSceneMgr()->destroyEntity(mDoor);
    mEngine->getSceneMgr()->destroySceneNode(mDoorNode);
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
