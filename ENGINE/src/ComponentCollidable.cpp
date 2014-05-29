#include "ComponentCollidable.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentCollidable::mType = new TypeInfo("ComponentCollidable", &ComponentCollidable::createInstance);

ComponentCollidable::ComponentCollidable(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    LOG_OUT("component");
}

ComponentCollidable::~ComponentCollidable()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentCollidable::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentCollidable(object, params);
}

bool ComponentCollidable::init()
{
    LOG_IN("component");
    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentCollidable::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentCollidable::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    LOG_OUT_MSG;
}
