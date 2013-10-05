#include "ComponentMover.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "MessageMove.hpp"

#include "OGRE/OgreStringConverter.h"

#include <stdlib.h>

using namespace ENGINE;

TypeInfo *ComponentMover::mType = new TypeInfo("ComponentMover", &ComponentMover::createInstance);

ComponentMover::ComponentMover(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{
    mSpeed = atof(params["Speed"].c_str());
    printf("\tSpeed == %lf\n", mSpeed);
    mTranslation = Ogre::Vector3();
}

ComponentMover::~ComponentMover()
{
}

void *ComponentMover::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentMover(object, params);
}

void ComponentMover::init()
{
}
    
void ComponentMover::update(float elapsedTime)
{
    mObject->getSceneNode()->translate(mTranslation.normalisedCopy() * elapsedTime * mSpeed, Ogre::Node::TS_LOCAL);
}

void ComponentMover::receiveMessage(Message *message)
{
    if(MessageStartMoveForward *m = dynamic_cast<MessageStartMoveForward *>(message))
    {
        mTranslation += Ogre::Vector3(0, 0, -1);
    }
    if(MessageStopMoveForward *m = dynamic_cast<MessageStopMoveForward *>(message))
    {
        mTranslation -= Ogre::Vector3(0, 0, -1);
    }

    if(MessageStartMoveBackward *m = dynamic_cast<MessageStartMoveBackward *>(message))
    {
        mTranslation += Ogre::Vector3(0, 0, 1);
    }
    if(MessageStopMoveBackward *m = dynamic_cast<MessageStopMoveBackward *>(message))
    {
        mTranslation -= Ogre::Vector3(0, 0, 1);
    }

    if(MessageStartMoveLeft *m = dynamic_cast<MessageStartMoveLeft *>(message))
    {
        mTranslation += Ogre::Vector3(-1, 0, 0);
    }
    if(MessageStopMoveLeft *m = dynamic_cast<MessageStopMoveLeft *>(message))
    {
        mTranslation -= Ogre::Vector3(-1, 0, 0);
    }

    if(MessageStartMoveRight *m = dynamic_cast<MessageStartMoveRight *>(message))
    {
        mTranslation += Ogre::Vector3(1, 0, 0);
    }
    if(MessageStopMoveRight *m = dynamic_cast<MessageStopMoveRight *>(message))
    {
        mTranslation -= Ogre::Vector3(1, 0, 0);
    }
}
