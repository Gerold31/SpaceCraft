#include "Component.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "MessageObject.hpp"

using namespace ENGINE;

Component::Component(Object *object, ParamMap &params, TypeInfo *type) :
    MessageReceiver(MessageReceiver::RECEIVER_COMPONENT),
    mObject(object),
    mParams(params),
    mType(type)
{
    LOG_IN("component");
    mReady = false;
    mEnable = true;
    LOG_OUT("component");
}


void Component::receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageEnable::getID())
    {
        mEnable = true;
    }
    else if(message->getID() == MessageDisable::getID())
    {
        mEnable = false;
        
        _receiveMessage(message);
    }

    if(!mEnable)
    {
        LOG_OUT_MSG;
        return;
    }

    _receiveMessage(message);

    LOG_OUT_MSG;
}