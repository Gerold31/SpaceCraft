#include "Kino.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"


Kino::Kino(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, "SC_Kino", engine)
{
    mEntity = engine->getSceneMgr()->createEntity("Kino.mesh");
    mNode->attachObject(mEntity);

    mCamera = engine->getSceneMgr()->createCamera(name + "Camera");
    mCamera->lookAt(0,0,-10);
    mCameraYawNode   = mNode->createChildSceneNode();
    mCameraPitchNode = mCameraYawNode->createChildSceneNode();
    mCameraRollNode  = mCameraPitchNode->createChildSceneNode();
    mCameraRollNode->attachObject(mCamera);

	mCamera->setNearClipDistance(1.5f);
	mCamera->setFarClipDistance(3000.0f); 
	mCamera->setAspectRatio(1.0f);

    mTranslation = Ogre::Vector3::ZERO;
}

bool Kino::update(float elapsedTime)
{
    if(mTranslation != Ogre::Vector3::ZERO)
	{
        float speed = 5.0f * elapsedTime;
        mNode->translate(mCameraYawNode->getOrientation() * mCameraPitchNode->getOrientation() * mTranslation.normalisedCopy() * speed, Ogre::Node::TS_LOCAL);
    }
    return true;
}

bool Kino::keyPressed(const OIS::KeyEvent &e)
{
    switch(e.key)
    {
    case OIS::KC_W:
        mTranslation.z = -1;
        break;
    case OIS::KC_S:
        mTranslation.z = 1;
        break;
    case OIS::KC_A:
		mTranslation.x = -1;
        break;
    case OIS::KC_D:
		mTranslation.x = 1;
        break;
    }
    return true;
}

bool Kino::keyReleased(const OIS::KeyEvent &e)
{
    switch(e.key)
    {
    case OIS::KC_W:
    case OIS::KC_S:
        mTranslation.z = 0;
        break;
    case OIS::KC_A:
    case OIS::KC_D:
		mTranslation.x = 0;
        break;
    }
    return true;
}
 
bool Kino::mouseMoved(const OIS::MouseEvent &e)
{
    //orientation
    float rotCoeff = -1.0f * 0.005;// elapsedTime;
    Ogre::Radian lAngleX(e.state.X.rel * rotCoeff);
    Ogre::Radian lAngleY(e.state.Y.rel * rotCoeff);
	// If the 'player' don't make loopings, 'yaw in world' + 'pitch in local' is often enough for a camera controler.
    mCameraYawNode->yaw(lAngleX);
    mCameraPitchNode->pitch(lAngleY);

    Ogre::Real pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(mCameraPitchNode->getOrientation().w)).valueDegrees());
 
    // Just to determine the sign of the angle we pick up above, the
    // value itself does not interest us.
    Ogre::Real pitchAngleSign = mCameraPitchNode->getOrientation().x;
 
    // Limit the pitch between -90 degress and +90 degrees, Quake3-style.
    if (pitchAngle > 90.0f)
    {
        if (pitchAngleSign > 0)
            // Set orientation to 90 degrees on X-axis.
             mCameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
        else if (pitchAngleSign < 0)
            // Sets orientation to -90 degrees on X-axis.
            mCameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
    }
    return true;
}

bool Kino::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

bool Kino::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}