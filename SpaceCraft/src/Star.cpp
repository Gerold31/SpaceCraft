#include "Star.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"

Star::Star(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :GravityObject(1e20, pos, ori, parent, name, "SC_Star", engine)
{
    mEntity = engine->getSceneMgr()->createEntity("Star.mesh");
    mNode->attachObject(mEntity);
}