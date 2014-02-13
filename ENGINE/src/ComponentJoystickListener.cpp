#include "ComponentJoystickListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"
#include "SystemLog.hpp"
#include "MessageInput.hpp"

using namespace ENGINE;

TypeInfo *ComponentJoystickListener::mType = new TypeInfo("ComponentJoystickListener", &ComponentJoystickListener::createInstance);

ComponentJoystickListener::ComponentJoystickListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mEnabled = true;
    LOG_OUT("component");
}

ComponentJoystickListener::~ComponentJoystickListener()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentJoystickListener::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentJoystickListener(object, params);
}

void ComponentJoystickListener::init()
{
    LOG_IN("component");
    SystemInput::getSingleton()->addJoystickListener(this);
    LOG_OUT("component");
}
    
void ComponentJoystickListener::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentJoystickListener::receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageJoystickEnable::getID())
    {
        MessageJoystickEnable *msg = (MessageJoystickEnable *)message;
        enable(msg->mEnable);
    }
    LOG_OUT_MSG;
}
