#ifndef _GRAVITYOBJECT_HPP_
#define _GRAVITYOBJECT_HPP_

#include "Entity.hpp"

#include <vector>

class GravityObject : public Entity
{
public:
    GravityObject(double mass, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine);
    
    virtual bool update(float elapsedTime);
        
    double getMass() {return mMass;}
    Ogre::Vector3 getVelocity() {return mVelocity;}

    void applyForce(Ogre::Vector3 force, double time);

protected:
    double mMass;
    Ogre::Vector3 mVelocity;
};

#endif