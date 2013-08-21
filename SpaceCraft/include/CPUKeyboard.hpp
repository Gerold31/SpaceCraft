#ifndef _CPUKEYBOARD_HPP_
#define _CPUKEYBOARD_HPP_

#include "Hardware.hpp"
#include "Input.hpp"

namespace Ogre
{
    class Entity;
};
#include "OGRE/OgreString.h"
#include "OGRE/OgreTexture.h"

#include "OIS/OIS.h"

#include <queue>

class CPUKeyboard : public Hardware, public OIS::KeyListener
{
public:
    CPUKeyboard(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    CPUKeyboard(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);

    void setInput(Input *input) {mInput = input;}

    bool update(float elapsedTime);
    void setMaterial();
    
    void interrupt();

    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);

    static std::string getType(){return mType;}

private:
    static std::string mType;

    Input *mInput;

    std::queue<WORD > mKeyBuffer;
    WORD mInterruptMsg;

    OIS::KeyCode keyToCode(WORD key);
    WORD codeToKey(OIS::KeyCode code);

    static SpaceShipPartInfo mPartInfo[1];

    void commonConstructor();
};

#endif
