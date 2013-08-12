#ifndef _CPUDISPLAY_HPP_
#define _CPUDISPLAY_HPP_

#include "Hardware.hpp"

namespace Ogre
{
    class Entity;
    class Light;
};
#include "OGRE/OgreString.h"
#include "OGRE/OgreTexture.h"

class CPUDisplay : public Hardware
{
public:
    CPUDisplay(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    CPUDisplay(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);
    ~CPUDisplay();

    bool update(float elapsedTime);
    
    void interrupt();

private:
    Ogre::Entity *mEntity;
    Ogre::TexturePtr mTexture;
    Ogre::Light *mLight[4];

    unsigned short *mData;
    unsigned int *mFont;
    unsigned short *mPalette;
    unsigned int *mDefaultFont;         // @todo static?
    unsigned short *mDefaultPalette;    // @todo static?
    
    void commonConstructor();

    void draw();
    
    void initData();
    void initFont();
    void initPalette();

    static SpaceShipPartInfo mPartInfo[1];
};

#endif