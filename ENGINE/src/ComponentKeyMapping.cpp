#include "ComponentKeyMapping.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "MessageInput.hpp"
#include "MessageMove.hpp"
#include "MessageEngine.hpp"
#include "MessageUse.hpp"
#include "Engine.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentKeyMapping::mType = new TypeInfo("ComponentKeyMapping", &ComponentKeyMapping::createInstance);

ComponentKeyMapping::ComponentKeyMapping(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    LOG_OUT("component");
}

ComponentKeyMapping::~ComponentKeyMapping()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentKeyMapping::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentKeyMapping(object, params);
}

void ComponentKeyMapping::init()
{
    LOG_IN("component");
    LOG_OUT("component");
}
    
void ComponentKeyMapping::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentKeyMapping::receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageMouseMoved::getID())
    {
        MessageMouseMoved *m = (MessageMouseMoved *)message;
        MessageLookAtRel msg(m->mEvent.state.X.rel, m->mEvent.state.Y.rel);
        msg.sendTo(mObject);
    }else if(message->getID() == MessageKeyPressed::getID())
    {
        MessageKeyPressed *m = (MessageKeyPressed *)message;
        switch(m->mEvent.key)
        {
        case OIS::KC_W:
        {
            MessageStartMoveForward msg;
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_S:
        {
            MessageStartMoveBackward msg;
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_A:
        {
            MessageStartMoveLeft msg;
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_D:
        {
            MessageStartMoveRight msg;
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_E:
        {
            MessageUse msg(mObject->getName());
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_ESCAPE:
        {
            MessageQuit msg;
            msg.sendTo(Engine::getSingleton());
            break;
        }
        }
    }else if(message->getID() == MessageKeyReleased::getID())
    {
        MessageKeyReleased *m = (MessageKeyReleased *)message;
        switch(m->mEvent.key)
        {
        case OIS::KC_W:
        {
            MessageStopMoveForward msg;
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_S:
        {
            MessageStopMoveBackward msg;
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_A:
        {
            MessageStopMoveLeft msg;
            msg.sendTo(mObject);
            break;
        }
        case OIS::KC_D:
        {
            MessageStopMoveRight msg;
            msg.sendTo(mObject);
            break;
        }
        }
    }else if(message->getID() == MessageMousePressed::getID())
    {
        MessageMousePressed *m = (MessageMousePressed *)message;
        switch(m->mButtonID)
        {
        case OIS::MB_Right:
        {
            MessageUse msg(mObject->getName());
            msg.sendTo(mObject);
            break;
        }
        }
    }
    LOG_OUT_MSG;
}
