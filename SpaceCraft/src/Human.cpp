#include "Human.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreRay.h"

#include "ENGINE.hpp"
#include "SpaceShipPart.hpp"

std::string Human::mType = "SC_Human";

Human::Human(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, mType, engine)
{
    Ogre::SceneNode *node = mNode->createChildSceneNode(name + "HumanNode");
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "Human/Human.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    node->attachObject(mEntity);
    node->setPosition(0,-1.6,0);
    node->setOrientation(0, 0, 1, 0);
    node->scale(0.1, 0.1, 0.1);

    mRaySceneQuery = engine->getSceneMgr()->createRayQuery(Ogre::Ray());
}

Human::~Human()
{
    mEngine->getSceneMgr()->destroyEntity(mEntity);
}

bool Human::update(float elapsedTime)
{
    return true;
}

SpaceShipPart *Human::getStandOn()
{
	Ogre::Ray ray(mNode->getParentSceneNode()->getPosition() + mNode->getPosition(), Ogre::Vector3(0, -1, 0));
    mRaySceneQuery->setRay(ray);
    mRaySceneQuery->setSortByDistance(true);

    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator i = result.begin();

    while(i!=result.end())
    {
		if(i->distance > 2)
			return NULL;
		if(i->movable && i->movable->getMovableType() == "Entity" && i->movable->getName() != mNode->getName() + "Mesh")
			return (SpaceShipPart *)Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
		++i;
	}
	return NULL;
}