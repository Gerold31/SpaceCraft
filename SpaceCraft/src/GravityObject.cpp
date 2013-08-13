#include "GravityObject.hpp"

#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreStringConverter.h"

GravityObject::GravityObject(double mass, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine)
    :Entity(pos, ori, parent, name, type, engine)
{
    mMass = mass;
    mVelocity = velocity;
}

void GravityObject::applyForce(Ogre::Vector3 force, double time)
{
    Ogre::Vector3 acceleration = Ogre::Vector3::ZERO;
    acceleration = force / mMass;
    mVelocity += acceleration * time;
    mNode->setPosition(mNode->getPosition() + mVelocity * time);
}
