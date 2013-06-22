#ifndef _CPUKEYBOARD_HPP_
#define _CPUKEYBOARD_HPP_

#include "Hardware.hpp"

namespace Ogre
{
    class Entity;
};
#include "OGRE/OgreString.h"
#include "OGRE/OgreTexture.h"

class CPUKeyboard : public Hardware
{
public:
    CPUKeyboard(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void interrupt();

private:
    Ogre::Entity *mEntity;
};

#endif