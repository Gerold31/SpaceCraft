#ifndef _GRAVITYOBJECT_HPP_
#define _GRAVITYOBJECT_HPP_

#include "Entity.hpp"


class GravityObject : public Entity
{
public:
    GravityObject(double mass, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine);
    
    virtual bool update(float elapsedTime) = 0;

protected:
    double mMass;
};

#endif