#ifndef _SPACESHIPDESIGNER_HPP_
#define _SPACESHIPDESIGNER_HPP_

#include "OIS/OIS.h"

class ENGINE;
class SpaceShip;
class SpaceShipPart;
class SpaceShipDesignerGUI;


namespace Ogre
{
    class SceneNode;
    class Camera;
    class Viewport;
    class RaySceneQuery;
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

    void setSelectedPartType(int type);

private:
    Ogre::SceneNode *mNode;
    Ogre::SceneNode *mCameraYawNode;
    Ogre::SceneNode *mCameraPitchNode;
    Ogre::SceneNode *mCameraRollNode;
    Ogre::Camera    *mCamera;
    Ogre::Viewport  *mViewport;
	Ogre::RaySceneQuery* mRaySceneQuery;

    ENGINE *mEngine;

    SpaceShip *mSpaceShip;
    SpaceShipDesignerGUI *mGUI;

    int mSelectedPartType;
    int mNextPartID;

    std::vector<SpaceShipPart *> mPossibleParts;
    
    void initPossibleParts();
    void removePossibleParts();
    void updateSelectedPartType();

    void addPossibleParts(SpaceShipPart *part);
    void removePossiblePart(SpaceShipPart *part);
    
    void debugShip(int partType = -1);
    void debugPart(SpaceShipPart *part, int partType = -1);
};

#endif