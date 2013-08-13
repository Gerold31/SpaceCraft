#ifndef _KINOCONTROL_HPP_
#define _KINOCONTROL_HPP_

class Kino;

#include "Entity.hpp"

namespace Ogre
{
    class Entity;
};
#include "OGRE/OgreString.h"

class KinoControl : public Entity
{
public:
    KinoControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    Kino *getKino() {return mKino;}

    static std::string getType(){return mType;}

private:
    static std::string mType;

    Ogre::Entity* mEntity;
    Kino *mKino;
};

#endif
