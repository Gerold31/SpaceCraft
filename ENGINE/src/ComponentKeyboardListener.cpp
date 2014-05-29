#include "ComponentKeyboardListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"
#include "SystemLog.hpp"
#include "MessageInput.hpp"

using namespace ENGINE;

TypeInfo *ComponentKeyboardListener::mType = new TypeInfo("ComponentKeyboardListener", &ComponentKeyboardListener::createInstance);

ComponentKeyboardListener::ComponentKeyboardListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mEnabled = true;
    LOG_OUT("component");
}

ComponentKeyboardListener::~ComponentKeyboardListener()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentKeyboardListener::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentKeyboardListener(object, params);
}

bool ComponentKeyboardListener::init()
{
    LOG_IN("component");
    SystemInput::getSingleton()->addKeyListener(this);
    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentKeyboardListener::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentKeyboardListener::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageKeyboardEnable::getID())
    {
        MessageKeyboardEnable *msg = (MessageKeyboardEnable *)message;
        enable(msg->mEnable);
    }
    LOG_OUT_MSG;
}
