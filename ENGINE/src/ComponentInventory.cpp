#include "ComponentInventory.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentInventory::mType = new TypeInfo("ComponentInventory", &ComponentInventory::createInstance);

ComponentInventory::ComponentInventory(Object *object, ParamMap &params) :
    ComponentItemContainer(object, params, mType)
{
    LOG_IN("component");
    LOG_OUT("component");
}

ComponentInventory::~ComponentInventory()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentInventory::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentInventory(object, params);
}

bool ComponentInventory::init()
{
    LOG_IN("component");
    mReady = ComponentItemContainer::init();
    LOG_OUT("component");
    return mReady;
}

void ComponentInventory::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentInventory::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    LOG_OUT_MSG;
}
