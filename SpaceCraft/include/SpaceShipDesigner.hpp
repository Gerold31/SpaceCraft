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
    enum MODE
    {
        MODE_BUILD,
        MODE_WIRE,
        MODE_WIRELIGHT,
        MODE_WIREDOOR
    };

    SpaceShipDesigner(ENGINE *engine);

    void enterEditMode(SpaceShip *ship);
    void exitEditMode();
    
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
 
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    
    void setMode(MODE mode);
    void setSelectedPartType(int type);
    void setSelectedPartName(std::string name);
    void setSelectedFloorFrom(int from);
    void setSelectedFloorTo(int to);
    void enableSelectedFloorFrom(bool enabled);
    void enableSelectedFloorTo(bool enabled);

private:
    Ogre::SceneNode *mParentNode;
    Ogre::SceneNode *mNode;
    Ogre::Camera    *mCamera;
    Ogre::Viewport  *mViewport;
	Ogre::RaySceneQuery* mRaySceneQuery;

    ENGINE *mEngine;

    SpaceShip *mSpaceShip;
    SpaceShipPart *mSelectedPart;
    SpaceShipPart *mLinkFirst;
    SpaceShipDesignerGUI *mGUI;

    int mSelectedPartType;
    int mSelectedFloorFrom;
    int mSelectedFloorTo;
    bool mSelectedFloorFromEnabled;
    bool mSelectedFloorToEnabled;

    std::string mSelectedPartName;

    int mRotation;

    int mNextPartID;

    std::vector<SpaceShipPart *> mPossibleParts;
    
    MODE mMode;
    
    void initPossibleParts();
    void removePossibleParts();
    void updateVisibleParts();

    void addPossibleParts(SpaceShipPart *part);
    void removePossiblePart(SpaceShipPart *part);
    
    void debugShip(int partType = -1);
    void debugPart(SpaceShipPart *part, int partType = -1);
};

#endif