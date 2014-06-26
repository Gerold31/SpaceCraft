#include "ComponentHotbarGUI.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "MessageHotbar.hpp"
#include "SystemGUI.hpp"
#include "ComponentItemContainer.hpp"
#include "ComponentItem.hpp"
#include "Object.hpp"

using namespace ENGINE;

TypeInfo *ComponentHotbarGUI::mType = new TypeInfo("ComponentHotbarGUI", &ComponentHotbarGUI::createInstance);

ComponentHotbarGUI::ComponentHotbarGUI(Object *object, ParamMap &params) :
    ComponentItemContainerGUI(object, params, mType)
{
    LOG_IN("component");
    mParams["ContainerType"] = std::string("ComponentHotbar");
    mEnable = false;
    mIndex = 0;
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
    {
        setVisible(true);
        selectIndex(0);
    }
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
    if(message->getID() == MessageNextSlot::getID())
    {
        selectIndex(mIndex+1);
    }else if(message->getID() == MessagePrevSlot::getID())
    {
        selectIndex(mIndex-1);
    }else if(message->getID() == MessageSlot0::getID())
    {
        selectIndex(0);
    }else if(message->getID() == MessageSlot1::getID())
    {
        selectIndex(1);
    }else if(message->getID() == MessageSlot2::getID())
    {
        selectIndex(2);
    }else if(message->getID() == MessageSlot3::getID())
    {
        selectIndex(3);
    }else if(message->getID() == MessageSlot4::getID())
    {
        selectIndex(4);
    }else if(message->getID() == MessageSlot5::getID())
    {
        selectIndex(5);
    }else if(message->getID() == MessageSlot6::getID())
    {
        selectIndex(6);
    }else if(message->getID() == MessageSlot7::getID())
    {
        selectIndex(7);
    }else if(message->getID() == MessageSlot8::getID())
    {
        selectIndex(8);
    }else if(message->getID() == MessageSlot9::getID())
    {
        selectIndex(9);
    }else
        ComponentItemContainerGUI::_receiveMessage(message);
    LOG_OUT_MSG;
}

void ComponentHotbarGUI::selectIndex(int index)
{
    LOG_IN("component");
    mItemBox->getChildAt(mIndex)->setUserString("Active", "false");
    mItemBox->redrawItemAt(mIndex);
    mIndex = index % mContainer->getNumberItems();
    mItemBox->getChildAt(mIndex)->setUserString("Active", "true");
    mItemBox->redrawItemAt(mIndex);

    MessageSelectHotbarItem m(mContainer->getItem(index)->getObject()->getName());
    m.sendTo(mObject);

    LOG_OUT("component");
}
