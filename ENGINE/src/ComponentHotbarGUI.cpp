#include "ComponentHotbarGUI.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "MessageInventory.hpp"
#include "SystemGUI.hpp"

using namespace ENGINE;

TypeInfo *ComponentHotbarGUI::mType = new TypeInfo("ComponentHotbarGUI", &ComponentHotbarGUI::createInstance);

ComponentHotbarGUI::ComponentHotbarGUI(Object *object, ParamMap &params) :
    ComponentItemContainerGUI(object, params, mType)
{
    LOG_IN("component");
    mParams["ContainerType"] = std::string("ComponentHotbar");
    mEnable = false;
    LOG_OUT("component");
}

ComponentHotbarGUI::~ComponentHotbarGUI()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentHotbarGUI::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentHotbarGUI(object, params);
}

bool ComponentHotbarGUI::init()
{
    LOG_IN("component");
    mReady = ComponentItemContainerGUI::init();
    if(mReady)
        setVisible(true);
    LOG_OUT("component");
    return mReady;
}

void ComponentHotbarGUI::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentHotbarGUI::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    ComponentItemContainerGUI::_receiveMessage(message);
    LOG_OUT_MSG;
}
