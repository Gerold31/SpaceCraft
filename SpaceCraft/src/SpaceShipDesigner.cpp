#include "SpaceShipDesigner.hpp"

#include "ENGINE.hpp"
#include "SpaceShip.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRay.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreStringConverter.h"

SpaceShipDesigner::SpaceShipDesigner(ENGINE *engine)
{
    mEngine = engine;
    mNode = mEngine->getSceneMgr()->getRootSceneNode()->createChildSceneNode("SpaceShipDesignerNode");
    mNode->setPosition(0, 10, 0);

    mCamera = mEngine->getSceneMgr()->createCamera("SpaceShipDesignerCamera");
    mCamera->lookAt(0, -1, 0);
    mCamera->setNearClipDistance(0.5);
    mCamera->setFarClipDistance(5000.0);

    mCameraYawNode   = mNode->createChildSceneNode();
    mCameraPitchNode = mCameraYawNode->createChildSceneNode();
    mCameraRollNode  = mCameraPitchNode->createChildSceneNode();
    mCameraRollNode->attachObject(mCamera);
}

void SpaceShipDesigner::enterEditMode(SpaceShip *ship) 
{
    mSpaceShip = ship;
    mViewport = mEngine->getWindow()->addViewport(mCamera, 100, 0, 0, 1, 1);
    
    mViewport->setAutoUpdated(true);
    mViewport->setBackgroundColour(Ogre::ColourValue(.1,.2,.9));
    mCamera->setAspectRatio(1.0f * mViewport->getActualWidth() / mViewport->getActualHeight());
}

void SpaceShipDesigner::exitEditMode()
{
    mEngine->getWindow()->removeViewport(100);
}

bool SpaceShipDesigner::keyPressed(const OIS::KeyEvent &e)
{
    return true;
}

bool SpaceShipDesigner::keyReleased(const OIS::KeyEvent &e)
{
    return true;
} 

bool SpaceShipDesigner::mouseMoved(const OIS::MouseEvent &e)
{
    return true;
}

bool SpaceShipDesigner::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

bool SpaceShipDesigner::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}