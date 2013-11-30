#include "ComponentKeyboardListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"

using namespace ENGINE;

TypeInfo *ComponentKeyboardListener::mType = new TypeInfo("ComponentKeyboardListener", &ComponentKeyboardListener::createInstance);

ComponentKeyboardListener::ComponentKeyboardListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{

}

ComponentKeyboardListener::~ComponentKeyboardListener()
{
}

void *ComponentKeyboardListener::createInstance(Object *object, ParamMap &params)
{
    return new ComponentKeyboardListener(object, params);
}

void ComponentKeyboardListener::init()
{
    SystemInput::getSingleton()->addKeyListener(this);
}
    
void ComponentKeyboardListener::update(float elapsedTime)
{
}

void ComponentKeyboardListener::receiveMessage(Message *message)
{
}
