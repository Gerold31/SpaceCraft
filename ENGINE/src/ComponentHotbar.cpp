#include "ComponentHotbar.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentHotbar::mType = new TypeInfo("ComponentHotbar", &ComponentHotbar::createInstance);

ComponentHotbar::ComponentHotbar(Object *object, ParamMap &params) :
    ComponentItemContainer(object, params, mType)
{
	LOG_IN("component");
	LOG_OUT("component");
}

ComponentHotbar::~ComponentHotbar()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentHotbar::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentHotbar(object, params);
}

bool ComponentHotbar::init()
{
    LOG_IN("component");
    mReady = ComponentItemContainer::init();
    LOG_OUT("component");
    return mReady;
}

void ComponentHotbar::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentHotbar::_receiveMessage(Message *message)
{
	LOG_IN_MSG;
	LOG_OUT_MSG;
}
