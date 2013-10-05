#include "ComponentKeyMapping.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "MessageInput.hpp"
#include "MessageMove.hpp"

using namespace ENGINE;

TypeInfo *ComponentKeyMapping::mType = new TypeInfo("ComponentKeyMapping", &ComponentKeyMapping::createInstance);

ComponentKeyMapping::ComponentKeyMapping(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{

}

ComponentKeyMapping::~ComponentKeyMapping()
{
}

void *ComponentKeyMapping::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentKeyMapping(object, params);
}

void ComponentKeyMapping::init()
{
}
    
void ComponentKeyMapping::update(float elapsedTime)
{
}

void ComponentKeyMapping::receiveMessage(Message *message)
{
    if(MessageKeyPressed *m = dynamic_cast<MessageKeyPressed *>(message))
    {
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
        }
    }
    if(MessageKeyReleased *m = dynamic_cast<MessageKeyReleased *>(message))
    {
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
    }
}
