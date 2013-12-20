#include "ComponentMouseListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentMouseListener::mType = new TypeInfo("ComponentMouseListener", &ComponentMouseListener::createInstance);

ComponentMouseListener::ComponentMouseListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    LOG_OUT("component");
}

ComponentMouseListener::~ComponentMouseListener()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentMouseListener::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentMouseListener(object, params);
}

void ComponentMouseListener::init()
{
    LOG_IN("component");
    SystemInput::getSingleton()->addMouseListener(this);
    LOG_OUT("component");
}
    
void ComponentMouseListener::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentMouseListener::receiveMessage(Message *message)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}
