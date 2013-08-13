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

    static std::string getType(){return mType;}

private:
    static std::string mType;

    Ogre::Entity *mEntity;
	Ogre::RaySceneQuery* mRaySceneQuery;

    Human *mHolder;
};

#endif
