#ifndef _COMPONENTINVENTORYGUI_HPP_
#define _COMPONENTINVENTORYGUI_HPP_

#include "Component.hpp"

#include <MyGUI.h>

namespace ENGINE
{

class ComponentInventory;

class ComponentInventoryGUI : public Component
{
public:
    ComponentInventoryGUI(Object *object, ParamMap &params);
    ~ComponentInventoryGUI();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

	static TypeInfo *getType() { return mType; }

	MyGUI::ItemBox* getItemBox() { return mItemBox; }

private:
	void notifyStartDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result);
	void notifyRequestDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result);
	void notifyEndDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool result);
	void notifyDropState(MyGUI::DDContainer *sender, MyGUI::DDItemState state);
	void notifyNotifyItem(MyGUI::ItemBox *sender, const MyGUI::IBNotifyItemData &info);

    void requestCreateWidgetItem(MyGUI::ItemBox *sender, MyGUI::Widget *item);
    void requestDrawItem(MyGUI::ItemBox *sender, MyGUI::Widget *item, const MyGUI::IBDrawItemInfo &info);

	static TypeInfo *mType;

    ComponentInventory *mInventory;

    MyGUI::VectorWidgetPtr mLayoutRoot;
	MyGUI::ItemBox *mItemBox;
    MyGUI::Widget *mDragItem;
    MyGUI::IntPoint mDragOff;
    bool mDragging;

};

};

#endif
