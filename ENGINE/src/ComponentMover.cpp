#include "ComponentMover.hpp"

#include "ComponentCamera.hpp"
#include "Object.hpp"
#include "TypeInfo.hpp"
#include "MessageMove.hpp"

#include "OGRE/OgreStringConverter.h"

#include <stdlib.h>

using namespace ENGINE;

TypeInfo *ComponentMover::mType = new TypeInfo("ComponentMover", &ComponentMover::createInstance);

ComponentMover::ComponentMover(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    mSpeed = atof(boost::any_cast<std::string>(params["Speed"]).c_str());
    mTranslation = Ogre::Vector3(0, 0, 0);
    mYawNode = mPitchNode = nullptr;
}

ComponentMover::~ComponentMover()
{
}

void *ComponentMover::createInstance(Object *object, ParamMap &params)
{
    return new ComponentMover(object, params);
}

void ComponentMover::init()
{
    for(int i=0; i<mObject->getNumberComponents(); i++)
    {
        Component *c = mObject->getComponent(i);
        if(c->getType() == ComponentCamera::getType())
        {
            mYawNode = ((ComponentCamera *)c)->getYawNode();
            mPitchNode = ((ComponentCamera *)c)->getPitchNode();
        }
    }
}
    
void ComponentMover::update(float elapsedTime)
{
    if(mTranslation != Ogre::Vector3(0, 0, 0))
    {
        Ogre::Vector3 t;
        if(mYawNode && mPitchNode)
            t = mYawNode->getOrientation() * mPitchNode->getOrientation() * mTranslation.normalisedCopy() * elapsedTime * mSpeed;
        else 
            t = mTranslation.normalisedCopy() * elapsedTime * mSpeed;
    
        //if(t.length() != 0)
        {
            t = mObject->getSceneNode()->getPosition() + t;
            MessageSetPosition m(t.x, t.y, t.z);
            std::cout << "send message" << std::endl;
            m.sendTo(mObject);
        }
    }
}

void ComponentMover::receiveMessage(Message *message)
{
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
    }
}
