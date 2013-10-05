#include "ComponentJoystickListener.hpp"

#include "TypeInfo.hpp"

using namespace ENGINE;

TypeInfo *ComponentJoystickListener::mType = new TypeInfo("ComponentJoystickListener", &ComponentJoystickListener::createInstance);

ComponentJoystickListener::ComponentJoystickListener(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{

}

ComponentJoystickListener::~ComponentJoystickListener()
{
}

void *ComponentJoystickListener::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentJoystickListener(object, params);
}

void ComponentJoystickListener::init()
{
}
    
void ComponentJoystickListener::update(float elapsedTime)
{
}

void ComponentJoystickListener::receiveMessage(Message *message)
{
}
