#include "ComponentItemContainerGUI.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "ComponentItemContainer.hpp"
#include "ComponentItem.hpp"
#include "Object.hpp"
#include "SystemGUI.hpp"
#include "SystemGraphics.hpp"
#include "SystemObjectFactory.hpp"
#include "MessageInput.hpp"

using namespace ENGINE;

#define ITEM_SIZE_X (70)
#define ITEM_SIZE_Y (70)

ComponentItemContainerGUI::ComponentItemContainerGUI(Object *object, ParamMap &params, TypeInfo *type) :
    Component(object, params, type)
{
	LOG_IN("component");
    mItemBox = nullptr;
    mDragging = false;
    mContainer = nullptr;
	LOG_OUT("component");
}

ComponentItemContainerGUI::~ComponentItemContainerGUI()
{
	LOG_IN("component");
	LOG_OUT("component");
}

bool ComponentItemContainerGUI::init()
{
	LOG_IN("component");
    if(!SystemGUI::getSingleton()->isInit())
    {
        LOG("SystemGUI not inited.", "component");
        LOG_OUT("component");
        return false;
    }
    Component *c = mObject->getComponent(SystemObjectFactory::getSingleton()->getComponentType(boost::any_cast<std::string>(mParams.at("ContainerType"))));
    if(c)
        mContainer = (ComponentItemContainer *)c;
    if(!mContainer)
    {
        LOG("No ItemContainer found!", "error");
        mReady = true;
	    LOG_OUT("component");
        return true;
    }

    if(!mContainer->isReady())
    {
        LOG("Inventory not inited.", "component");
	    LOG_OUT("component");
        return false;
    }

    std::string layout = boost::any_cast<std::string>(mParams["Layout"]);
    if(layout == "")
    {
        LOG("No Layout specified!", "error");
        mReady = true;
	    LOG_OUT("component");
        return true;
    }
    mLayoutRoot = SystemGUI::getSingleton()->loadLayout(layout);
    if(mLayoutRoot.size() == 0)
    {
        LOG("Layout not found!", "error");
        mReady = true;
	    LOG_OUT("component");
        return true;
    }

    for(size_t i=0; i<mLayoutRoot.at(0)->getChildCount(); i++)
    {
        if(mLayoutRoot.at(0)->getChildAt(i)->getTypeName() == "ItemBox")
        {
            mItemBox = (MyGUI::ItemBox *)mLayoutRoot.at(0)->getChildAt(i);
            break;
        }
    }
    if(!mItemBox)
    {
        LOG("No ItemBox in Layout found!", "error");
        mReady = true;
	    LOG_OUT("component");
        return true;
    }
    
    mItemBox->eventStartDrag += MyGUI::newDelegate(this, &ComponentItemContainerGUI::notifyStartDrop);
    mItemBox->eventRequestDrop += MyGUI::newDelegate(this, &ComponentItemContainerGUI::notifyRequestDrop);
    mItemBox->eventDropResult += MyGUI::newDelegate(this, &ComponentItemContainerGUI::notifyEndDrop);
    mItemBox->eventChangeDDState += MyGUI::newDelegate(this, &ComponentItemContainerGUI::notifyDropState);
    mItemBox->eventNotifyItem += MyGUI::newDelegate(this, &ComponentItemContainerGUI::notifyNotifyItem);
    mItemBox->requestCreateWidgetItem = MyGUI::newDelegate(this, &ComponentItemContainerGUI::requestCreateWidgetItem);
    mItemBox->requestDrawItem = MyGUI::newDelegate(this, &ComponentItemContainerGUI::requestDrawItem);

    for(size_t i=0; i<mContainer->getNumberItems(); i++)
    {
        mItemBox->addItem(mContainer->getItem(i));
    }
    
    mLayoutRoot.at(0)->setSize(SystemGraphics::getSingleton()->getWindow()->getWidth(), SystemGraphics::getSingleton()->getWindow()->getHeight());
    mLayoutRoot.at(0)->setVisible(false);

    mReady = true;
	LOG_OUT("component");
    return true;
}
    
void ComponentItemContainerGUI::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentItemContainerGUI::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageMouseMoved::getID())
    {
        if(mDragging)
            mDragItem->setPosition(SystemGUI::getSingleton()->getMousePos() - mItemBox->getPosition() - mDragOff);
    }
    LOG_OUT_MSG;
}

void ComponentItemContainerGUI::requestCreateWidgetItem(MyGUI::ItemBox* sender, MyGUI::Widget* item)
{
	LOG_IN("component");
    assert(sender == mItemBox);
    SystemGUI::getSingleton()->loadLayout("Item.layout").at(0)->attachToWidget(item);
    item->attachToWidget(mItemBox);

    if(mItemBox->getChildCount() == mContainer->getNumberItems() + 1)
    {
        mDragItem = item;
        item->setNeedMouseFocus(false);
    }

	LOG_OUT("component");
}

void ComponentItemContainerGUI::requestDrawItem(MyGUI::ItemBox* sender, MyGUI::Widget* item, const MyGUI::IBDrawItemInfo& info)
{
    LOG_IN_FRAME;
    ComponentItem *c = *sender->getItemDataAt<ComponentItem *>(info.index);

    item->setPosition((info.index%8)*ITEM_SIZE_X, (info.index/8)*ITEM_SIZE_Y);
    item->setSize(ITEM_SIZE_X, ITEM_SIZE_Y);
    
    for(size_t i=0; i<item->getChildAt(0)->getChildCount(); i++)
    {
        MyGUI::Widget *w = item->getChildAt(0)->getChildAt(i);
        if(w->getName() == "image_border")
        {
            char size[16];
            
            if(info.drop_accept)
            {
                sprintf(size, "0 %d %d %d", 2*ITEM_SIZE_Y, ITEM_SIZE_X, ITEM_SIZE_Y);
                w->setProperty("ImageCoord", size);
            }else if(info.drop_refuse)
            {
                sprintf(size, "0 %d %d %d", ITEM_SIZE_Y, ITEM_SIZE_X, ITEM_SIZE_Y);
                w->setProperty("ImageCoord", size);
            }else
            {
                sprintf(size, "0 0 %d %d", ITEM_SIZE_X, ITEM_SIZE_Y);
                w->setProperty("ImageCoord", size);
            }
        }else if(w->getName() == "text_count")
        {
            if(c->getCount() > 1 && !info.drag)
                w->setProperty("Caption", MyGUI::utility::toString(c->getCount()));
            else
                w->setProperty("Caption", "");
        }else if(w->getName() == "image_item")
        {
            if(c->getCount() == 0)
                w->setProperty("ImageTexture", "");
            else
                w->setProperty("ImageTexture", c->getName() + ".png");
        }
    }
    LOG_OUT_FRAME;
}

void ComponentItemContainerGUI::notifyStartDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result)
{
    LOG_IN_FRAME;
	if (info.sender_index != MyGUI::ITEM_NONE)
	{
		ComponentItem* data = *static_cast<MyGUI::ItemBox*>(info.sender)->getItemDataAt<ComponentItem*>(info.sender_index);
		result = !data->isEmpty();
        
        mDragging = result;
	}
    LOG_OUT_FRAME;
}

void ComponentItemContainerGUI::notifyRequestDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result)
{
    LOG_IN_FRAME;
	if (info.receiver_index == MyGUI::ITEM_NONE)
	{
		result = false;
        LOG_IN_FRAME;
		return;
	}

	if ((info.sender == info.receiver) && (info.sender_index == info.receiver_index))
	{
		result = false;
    LOG_OUT_FRAME;
		return;
	}

	ComponentItem *sender_data = *static_cast<MyGUI::ItemBox*>(info.sender)->getItemDataAt<ComponentItem*>(info.sender_index);
	ComponentItem *receiver_data = *static_cast<MyGUI::ItemBox*>(info.receiver)->getItemDataAt<ComponentItem*>(info.receiver_index);

	result = receiver_data->isEmpty() || receiver_data->compare(sender_data);
    LOG_OUT_FRAME;
}

void ComponentItemContainerGUI::notifyEndDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool result)
{
    LOG_IN_FRAME;
	if (result)
	{
		ComponentItem *sender_data = *static_cast<MyGUI::ItemBox*>(info.sender)->getItemDataAt<ComponentItem*>(info.sender_index);
		ComponentItem *receiver_data = *static_cast<MyGUI::ItemBox*>(info.receiver)->getItemDataAt<ComponentItem*>(info.receiver_index);

		sender_data->move(receiver_data);

		static_cast<MyGUI::ItemBox*>(info.receiver)->setItemData(info.receiver_index, receiver_data);
		static_cast<MyGUI::ItemBox*>(info.sender)->setItemData(info.sender_index, sender_data);
        
        mDragging = result;
	}
    LOG_OUT_FRAME;
}

void ComponentItemContainerGUI::notifyDropState(MyGUI::DDContainer *sender, MyGUI::DDItemState state)
{
    LOG_IN_FRAME;
	/*if (_state == MyGUI::DDItemState::Refuse) MyGUI::PointerManager::getInstance().setPointer("drop_refuse", sender->mainWidget());
	else if (_state == MyGUI::DDItemState::Accept) MyGUI::PointerManager::getInstance().setPointer("drop_accept", sender->mainWidget());
	else if (_state == MyGUI::DDItemState::Miss) MyGUI::PointerManager::getInstance().setPointer("drop", sender->mainWidget());
	else if (_state == MyGUI::DDItemState::Start) MyGUI::PointerManager::getInstance().setPointer("drop", sender->mainWidget());
	else if (_state == MyGUI::DDItemState::End) MyGUI::PointerManager::getInstance().setDefaultPointer();*/
    LOG_OUT_FRAME;
}

void ComponentItemContainerGUI::notifyNotifyItem(MyGUI::ItemBox *sender, const MyGUI::IBNotifyItemData &info)
{
    LOG_IN_FRAME;
	if(info.index != MyGUI::ITEM_NONE)
	{
	    if(info.notify == MyGUI::IBNotifyItemData::MousePressed)
	    {
            mDragOff = SystemGUI::getSingleton()->getMousePos() - mItemBox->getPosition() - mItemBox->getChildAt(info.index)->getPosition();
	    }
	}
    LOG_OUT_FRAME;
}

void ComponentItemContainerGUI::setVisible(bool visible)
{
    mLayoutRoot.at(0)->setVisible(visible);
}
