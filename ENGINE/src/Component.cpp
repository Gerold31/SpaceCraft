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
    mName = boost::any_cast<std::string>(params["Name"]);
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
    else if(message->getID() == MessageEnableComponent::getID())
    {
        MessageEnableComponent *m = (MessageEnableComponent *)message;
        if(mName == m->mName)
        {
            mEnable = true;
        }
    }
    else if(message->getID() == MessageDisableComponent::getID())
    {
        MessageDisableComponent *m = (MessageDisableComponent *)message;
        if(mName == m->mName)
        {
            mEnable = false;
        
            _receiveMessage(message);
        }
    }

    if(!mEnable)
    {
        LOG_OUT_MSG;
        return;
    }

    _receiveMessage(message);

    LOG_OUT_MSG;
}