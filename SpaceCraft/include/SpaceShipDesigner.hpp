#ifndef _SPACESHIPDESIGNER_HPP_
#define _SPACESHIPDESIGNER_HPP_

#include "OIS/OIS.h"

class ENGINE;
class SpaceShip;

namespace Ogre
{
    class SceneNode;
    class Camera;
    class Viewport;
};

class SpaceShipDesigner : public OIS::KeyListener, public OIS::MouseListener
{
public:
    SpaceShipDesigner(ENGINE *engine);

    void enterEditMode(SpaceShip *ship);
    void exitEditMode();
    
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
 
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
private:
    Ogre::SceneNode *mNode;
    Ogre::SceneNode *mCameraYawNode;
    Ogre::SceneNode *mCameraPitchNode;
    Ogre::SceneNode *mCameraRollNode;
    Ogre::Camera    *mCamera;
    Ogre::Viewport  *mViewport;

    ENGINE *mEngine;

    SpaceShip *mSpaceShip;
};

#endif