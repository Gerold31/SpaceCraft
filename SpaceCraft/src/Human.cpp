#include "Human.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreEntity.h"

#include "ENGINE.hpp"

Human::Human(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, "SC_Human", engine)
{
    Ogre::SceneNode *node = mNode->createChildSceneNode(name + "HumanNode");
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "Human/Human.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    node->attachObject(mEntity);
    node->setPosition(0,-1.6,0);
    node->setOrientation(0, 0, 1, 0);
    node->scale(0.1, 0.1, 0.1);
}

Human::~Human()
{
    mEngine->getSceneMgr()->destroyEntity(mEntity);
}

bool Human::update(float elapsedTime)
{
    return true;
}
