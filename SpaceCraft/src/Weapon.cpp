#include "Weapon.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

#include "Human.hpp"

Weapon::Weapon(Human *holder, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, "SC_Weapon", engine)
{
    mEntity = engine->getSceneMgr()->createEntity("Handgun.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    mHolder = holder;

    mRaySceneQuery = engine->getSceneMgr()->createRayQuery(Ogre::Ray());
}

bool Weapon::update(float elapsedTime)
{
    return true;
}

void  Weapon::shoot()
{
    Ogre::Ray ray(mNode->_getDerivedPosition(), mNode->_getDerivedOrientation() * Ogre::Vector3(0, 0, -1));
    mRaySceneQuery->setRay(ray);
    mRaySceneQuery->setSortByDistance(true);

    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator i = result.begin();

    while(i!=result.end())
    {            
        if(i->movable && i->movable->getMovableType() == "Entity")
        {
            Entity *obj = Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
            if(obj && obj != this && obj != mHolder)
            {
                printf("shot at %s\n", obj->getName().c_str());
            }
        }
        ++i;
    }
}