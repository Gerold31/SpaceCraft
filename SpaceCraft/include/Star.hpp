#ifndef _STAR_HPP_
#define _STAR_HPP_

#include "GravityObject.hpp"

class Star : public GravityObject
{
public:
    Star(double mass, double radius, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    static std::string getType(){return mType;}

private:
    static std::string mType;

    Ogre::Entity *mEntity;
};

#endif
