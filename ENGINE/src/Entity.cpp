#include "Entity.hpp"

#include "ENGINE.hpp"
#include "Map.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"

Entity::Entity(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine)
    : Ogre::ManualObject(name + "Obj"),
    mEngine(engine),
    mType(type)
{
    mNode = parent->createChildSceneNode(name);
    mNode->setPosition(pos);
    mNode->setOrientation(ori);
    mNode->getUserObjectBindings().setUserAny("Entity", Ogre::Any(this));
    mNode->attachObject(this);
    mMap = engine->getMap();
    mMap->registerEntity(this);
}

Entity::~Entity()
{
    mEngine->getSceneMgr()->destroySceneNode(mNode);
    mNode = NULL;
}