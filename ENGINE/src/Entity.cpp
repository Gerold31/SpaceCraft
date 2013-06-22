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
    mMap = engine->getMap();
    mNode = parent->createChildSceneNode(name);
    mNode->setPosition(pos);
    mNode->setOrientation(ori);
    mNode->attachObject(this);
    mMap->registerEntity(this);
}