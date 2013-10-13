#include "ComponentCamera.hpp"

#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "TypeInfo.hpp"
#include "MessageMove.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

TypeInfo *ComponentCamera::mType = new TypeInfo("ComponentCamera", &ComponentCamera::createInstance);

ComponentCamera::ComponentCamera(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{
    mCamera = SystemGraphics::getSingleton()->getSceneMgr()->createCamera(object->getName() + "Camera");
    mCamera->lookAt(0,0,-1);
    mCamera->setPosition(0, 0, 0);
    mCamera->setNearClipDistance(0.3);
    mCamera->setFarClipDistance(300.0);

    mCameraYawNode = object->getSceneNode()->createChildSceneNode();
    mCameraPitchNode = mCameraYawNode->createChildSceneNode();
    mCameraRollNode = mCameraPitchNode->createChildSceneNode();

    mCameraRollNode->attachObject(mCamera);
}

ComponentCamera::~ComponentCamera()
{
}

void *ComponentCamera::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentCamera(object, params);
}

void ComponentCamera::init()
{
}
    
void ComponentCamera::update(float elapsedTime)
{
}

void ComponentCamera::receiveMessage(Message *message)
{
    if(MessageLookAtRel *m = dynamic_cast<MessageLookAtRel *>(message))
    {
        //orientation
        float rotCoeff = -1.0f * 0.005;// elapsedTime;
        Ogre::Radian lAngleX(m->mX * rotCoeff);
        Ogre::Radian lAngleY(m->mY * rotCoeff);
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
    }
}
