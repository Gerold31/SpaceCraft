#include "ComponentMover.hpp"

#include "ComponentCamera.hpp"
#include "Object.hpp"
#include "TypeInfo.hpp"
#include "MessageMove.hpp"
#include "SystemLog.hpp"

#include "OGRE/OgreStringConverter.h"

#include <stdlib.h>

using namespace ENGINE;

#define SYNC_TIME (1/30.0)

TypeInfo *ComponentMover::mType = new TypeInfo("ComponentMover", &ComponentMover::createInstance);

ComponentMover::ComponentMover(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mSpeed = atof(boost::any_cast<std::string>(params["Speed"]).c_str());
    mTranslation = Ogre::Vector3(0, 0, 0);

    mElapsedTime = 0.0;
    mYawNode = mPitchNode = mRollNode = nullptr;
    LOG_OUT("component");
}

ComponentMover::~ComponentMover()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentMover::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentMover(object, params);
}

void ComponentMover::init()
{
    LOG_IN("component");
    mYawNode = mObject->getSceneNode()->createChildSceneNode(mObject->getName() + "YawNode");
    mPitchNode = mYawNode->createChildSceneNode(mObject->getName() + "PitchNode");
    mRollNode = mPitchNode->createChildSceneNode(mObject->getName() + "RollNode");
    LOG_OUT("component");
}
    
void ComponentMover::update(float elapsedTime)
{
    LOG_IN_FRAME;
    mElapsedTime += elapsedTime;
    if(mTranslation != Ogre::Vector3(0, 0, 0))
    {
        Ogre::Vector3 t;
        t = mYawNode->getOrientation() * mPitchNode->getOrientation() * mTranslation.normalisedCopy() * elapsedTime * mSpeed;
    
        t = mObject->getSceneNode()->getPosition() + t;
        MessageSetPosition m(t.x, t.y, t.z);
        mObject->receiveMessage(&m);

        if(mElapsedTime > SYNC_TIME)
        {
            mElapsedTime = 0.0;
            m.sendTo(mObject);
        }
    }
    LOG_OUT_FRAME;
}

void ComponentMover::receiveMessage(Message *message)
{
    LOG_IN_FRAME;
    if(MessageStartMoveForward *m = dynamic_cast<MessageStartMoveForward *>(message))
    {
        mTranslation += Ogre::Vector3(0, 0, -1);
    }else if(MessageStopMoveForward *m = dynamic_cast<MessageStopMoveForward *>(message))
    {
        mTranslation -= Ogre::Vector3(0, 0, -1);
    }
    else if(MessageStartMoveBackward *m = dynamic_cast<MessageStartMoveBackward *>(message))
    {
        mTranslation += Ogre::Vector3(0, 0, 1);
    }else if(MessageStopMoveBackward *m = dynamic_cast<MessageStopMoveBackward *>(message))
    {
        mTranslation -= Ogre::Vector3(0, 0, 1);
    }
    else if(MessageStartMoveLeft *m = dynamic_cast<MessageStartMoveLeft *>(message))
    {
        mTranslation += Ogre::Vector3(-1, 0, 0);
    }else if(MessageStopMoveLeft *m = dynamic_cast<MessageStopMoveLeft *>(message))
    {
        mTranslation -= Ogre::Vector3(-1, 0, 0);
    }
    else if(MessageStartMoveRight *m = dynamic_cast<MessageStartMoveRight *>(message))
    {
        mTranslation += Ogre::Vector3(1, 0, 0);
    }else if(MessageStopMoveRight *m = dynamic_cast<MessageStopMoveRight *>(message))
    {
        mTranslation -= Ogre::Vector3(1, 0, 0);
    }else if(message->getID() == MessageLookAtRel::getID())
    {
        MessageLookAtRel *m = (MessageLookAtRel *)message;
        //orientation
        float rotCoeff = -1.0f * 0.005;// elapsedTime;
        Ogre::Radian lAngleX(m->mX * rotCoeff);
        Ogre::Radian lAngleY(m->mY * rotCoeff);
	    // If the 'player' don't make loopings, 'yaw in world' + 'pitch in local' is often enough for a camera controler.
        mYawNode->yaw(lAngleX);
        mPitchNode->pitch(lAngleY);

        Ogre::Real pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(mPitchNode->getOrientation().w)).valueDegrees());
 
        // Just to determine the sign of the angle we pick up above, the
        // value itself does not interest us.
        Ogre::Real pitchAngleSign = mPitchNode->getOrientation().x;
 
        // Limit the pitch between -90 degress and +90 degrees, Quake3-style.
        if (pitchAngle > 90.0f)
        {
            if (pitchAngleSign > 0)
                // Set orientation to 90 degrees on X-axis.
                mPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
            else if (pitchAngleSign < 0)
                // Sets orientation to -90 degrees on X-axis.
                mPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
        }
    }
    LOG_OUT_FRAME;
}
