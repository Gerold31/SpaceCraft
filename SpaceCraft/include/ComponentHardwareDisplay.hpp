#ifndef _COMPONENTHARDWAREDISPLAY_HPP_
#define _COMPONENTHARDWAREDISPLAY_HPP_

#include "ComponentHardware.hpp"

namespace Ogre
{
    class Entity;
    class Light;
};
#include "OGRE/OgreString.h"
#include "OGRE/OgreTexture.h"

namespace SpaceCraft
{

class ComponentHardwareDisplay : public ComponentHardware
{
public:
    ComponentHardwareDisplay(Object *object, ParamMap &params);
    ~ComponentHardwareDisplay();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
	void interrupt();
    
    void renderImage();
    
    void initData();
    void initFont();
    void initPalette();
    
    Ogre::Entity *mEntity;
    Ogre::TexturePtr mTexture;
    Ogre::Light *mLight[4];
    Ogre::Image mImage;

    unsigned short *mData;
    unsigned int *mFont;
    unsigned short *mPalette;
    unsigned int *mDefaultFont;         // @todo static?
    unsigned short *mDefaultPalette;    // @todo static?

    float mTimeSinceFrame;
	
    static TypeInfo *mType;

};

};

#endif
