#ifndef _STAR_HPP_
#define _STAR_HPP_

#include "GravityObject.hpp"

class Star : public GravityObject
{
public:
    Star(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    
    virtual bool update(float elapsedTime){return true;}

private:
    Ogre::Entity *mEntity;
};

#endif