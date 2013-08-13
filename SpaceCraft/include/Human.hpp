#ifndef _HUMAN_HPP_
#define _HUMAN_HPP_

namespace Ogre
{
    class Entity;
};

#include "OGRE/OgreString.h"

#include "Entity.hpp"

class Human : public Entity
{
public:
    Human(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    ~Human();

    bool update(float elapsedTime);

    static std::string getType(){return mType;}

protected:
    static std::string mType;

    Ogre::Entity *mEntity;
};

#endif
