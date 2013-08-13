#include "Star.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"

std::string Star::mType = "SC_Star";

Star::Star(double mass, double radius, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :GravityObject(mass, velocity, pos, ori, parent, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity("Star.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
    mNode->scale(radius, radius, radius);
}
