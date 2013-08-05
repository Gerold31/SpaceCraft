#ifndef _CPUDISPLAY_HPP_
#define _CPUDISPLAY_HPP_

#include "Hardware.hpp"

namespace Ogre
{
    class Entity;
};
#include "OGRE/OgreString.h"
#include "OGRE/OgreTexture.h"

class CPUDisplay : public Hardware
{
public:
    CPUDisplay(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void interrupt();

private:
    Ogre::Entity *mEntity;
    Ogre::TexturePtr mTexture;

    unsigned short *mData;
    unsigned int *mFont;
    unsigned short *mPalette;
    unsigned int *mDefaultFont;         // @todo static?
    unsigned short *mDefaultPalette;    // @todo static?

    void draw();
    
    void initData();
    void initFont();
    void initPalette();

    static SpaceShipPartInfo mPartInfo[1];
};

#endif