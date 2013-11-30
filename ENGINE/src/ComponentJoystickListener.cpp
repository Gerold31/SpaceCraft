#include "ComponentJoystickListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"

using namespace ENGINE;

TypeInfo *ComponentJoystickListener::mType = new TypeInfo("ComponentJoystickListener", &ComponentJoystickListener::createInstance);

ComponentJoystickListener::ComponentJoystickListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{

}

ComponentJoystickListener::~ComponentJoystickListener()
{
}

void *ComponentJoystickListener::createInstance(Object *object, ParamMap &params)
{
    return new ComponentJoystickListener(object, params);
}

void ComponentJoystickListener::init()
{
    SystemInput::getSingleton()->addJoystickListener(this);
}
    
void ComponentJoystickListener::update(float elapsedTime)
{
}

void ComponentJoystickListener::receiveMessage(Message *message)
{
}
