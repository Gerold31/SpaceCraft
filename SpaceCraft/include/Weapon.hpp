#ifndef _WEAPON_HPP_
#define _WEAPON_HPP_

#include "Entity.hpp"

class Human;

class Weapon : public Entity
{
public:
    Weapon(Human *holder, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void shoot();

private:
    Ogre::Entity *mEntity;
	Ogre::RaySceneQuery* mRaySceneQuery;

    Human *mHolder;
};

#endif