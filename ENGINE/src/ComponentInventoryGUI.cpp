#include "ComponentInventoryGUI.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "MessageInventory.hpp"
#include "SystemGUI.hpp"

using namespace ENGINE;

TypeInfo *ComponentInventoryGUI::mType = new TypeInfo("ComponentInventoryGUI", &ComponentInventoryGUI::createInstance);

ComponentInventoryGUI::ComponentInventoryGUI(Object *object, ParamMap &params) :
    ComponentItemContainerGUI(object, params, mType)
{
    LOG_IN("component");
    mParams["ContainerType"] = std::string("ComponentInventory");
    mEnable = false;
    LOG_OUT("component");
}

ComponentInventoryGUI::~ComponentInventoryGUI()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentInventoryGUI::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentInventoryGUI(object, params);
}

bool ComponentInventoryGUI::init()
{
    LOG_IN("component");
    mReady = ComponentItemContainerGUI::init();
    if(mReady)
        setVisible(false);
    LOG_OUT("component");
    return mReady;
}

void ComponentInventoryGUI::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentInventoryGUI::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageToggleInventory::getID())
    {
        mEnable = !mEnable;
        setVisible(mEnable);
        SystemGUI::getSingleton()->setMouseEnable(mEnable);
    }else
    {
        ComponentItemContainerGUI::_receiveMessage(message);
    }
    LOG_OUT_MSG;
}
