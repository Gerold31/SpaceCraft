#include "GravityObject.hpp"

GravityObject::GravityObject(double mass, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine)
    :Entity(pos, ori, parent, name, type, engine)
{
    mMass = mass;
}