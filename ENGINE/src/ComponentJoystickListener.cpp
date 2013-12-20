#include "ComponentJoystickListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentJoystickListener::mType = new TypeInfo("ComponentJoystickListener", &ComponentJoystickListener::createInstance);

ComponentJoystickListener::ComponentJoystickListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
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
}

void ComponentJoystickListener::receiveMessage(Message *message)
{
}
