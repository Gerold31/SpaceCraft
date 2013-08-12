#include "SpaceShipPartWindow.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLight.h"

#define OPEN_Z (1.0)
#define CLOSE_Z (0.0)

#define DOOR_SPEED (2.0)

SpaceShipPartWindow::SpaceShipPartWindow(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPartWall(pos, ori, parent, name, engine, "SC_SpaceShipPartWindow")
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "Window.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    commonConstructor();
}

SpaceShipPartWindow::SpaceShipPartWindow(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    : SpaceShipPartWall(pos, ori, parent, staticGeometry, name, engine, "SC_SpaceShipPartWindow")
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "Window.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);

    commonConstructor();
}

void SpaceShipPartWindow::commonConstructor()
{
}

SpaceShipPartWindow::SpaceShipPartWindow(SpaceShipPart *old, Ogre::String name)
    : SpaceShipPartWall(old, name, "SC_SpaceShipPartWindow")
{
    if(mEntity)
        mEngine->getSceneMgr()->destroyEntity(mEntity);
    mEntity = mEngine->getSceneMgr()->createEntity(name + "Mesh", "Window.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
}

bool SpaceShipPartWindow::update(float elapsedTime)
{
    return true;
}