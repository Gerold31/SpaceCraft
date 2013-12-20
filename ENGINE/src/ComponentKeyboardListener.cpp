#include "ComponentKeyboardListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentKeyboardListener::mType = new TypeInfo("ComponentKeyboardListener", &ComponentKeyboardListener::createInstance);

ComponentKeyboardListener::ComponentKeyboardListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
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

void ComponentKeyboardListener::init()
{
    LOG_IN("component");
    SystemInput::getSingleton()->addKeyListener(this);
    LOG_OUT("component");
}
    
void ComponentKeyboardListener::update(float elapsedTime)
{
}

void ComponentKeyboardListener::receiveMessage(Message *message)
{
}
