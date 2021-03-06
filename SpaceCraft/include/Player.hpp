#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

class Input;
class SpaceShip;
class SpaceShipDesigner;
class SpaceShipPart;
class Flashlight;
class Weapon;

#include "OIS/OIS.h"

namespace Ogre
{
    class SceneManager;
    class SceneNode;
    class Camera;
    class Viewport;
    class RenderWindow;
};

#include "OGRE/OgreString.h"

#include "Human.hpp"

class Player : public Human, public OIS::KeyListener, public OIS::MouseListener
{
public:
    Player(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::RenderWindow *window, Ogre::String name, ENGINE *engine);

    void setSpaceShip(SpaceShip *ship){mShip = ship;}

    bool update(float elapsedTime);

    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
 
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
    Input *mInput;

    Ogre::Vector3    mTranslation;
    Ogre::SceneNode *mCameraYawNode;
    Ogre::SceneNode *mCameraPitchNode;
    Ogre::SceneNode *mCameraRollNode;
    Ogre::Camera    *mCamera;
    Ogre::Viewport  *mViewport;

    enum MODE
    {
        MODE_DEFAULT,
        MODE_KINOCONTROL,
        MODE_KEYBOARD,
        MODE_DESIGN
    };
    MODE mMode;

    SpaceShip *mShip;
    SpaceShipDesigner *mSpaceShipDesigner;

    Flashlight *mFlashlight;
    Weapon *mWeapon;
};

#endif
