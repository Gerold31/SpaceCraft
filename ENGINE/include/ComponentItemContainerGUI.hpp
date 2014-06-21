#ifndef _COMPONENTITEMCONTAINERGUI_HPP_
#define _COMPONENTITEMCONTAINERGUI_HPP_

#include "Component.hpp"

#include <MyGUI.h>

namespace ENGINE
{

class ComponentItemContainer;

class ComponentItemContainerGUI : public Component
{
public:
    ComponentItemContainerGUI(Object *object, ParamMap &params, TypeInfo *type);
    ~ComponentItemContainerGUI();
    
    bool init();
    void update(float elapsedTime);

	MyGUI::ItemBox* getItemBox() { return mItemBox; }
    void setVisible(bool visible);

protected:
    void _receiveMessage(Message *message);

private:
    static void notifyStartDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result);
    static void notifyRequestDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result);
    static void notifyEndDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool result);
    static void notifyDropState(MyGUI::DDContainer *sender, MyGUI::DDItemState state);
    static void notifyNotifyItem(MyGUI::ItemBox *sender, const MyGUI::IBNotifyItemData &info);

    void requestCreateWidgetItem(MyGUI::ItemBox *sender, MyGUI::Widget *item);
    static void requestDrawItem(MyGUI::ItemBox *sender, MyGUI::Widget *item, const MyGUI::IBDrawItemInfo &info);

    ComponentItemContainer *mContainer;

    MyGUI::VectorWidgetPtr mLayoutRoot;
	MyGUI::ItemBox *mItemBox;
    static MyGUI::Widget *mDragItem;
    static MyGUI::IntPoint mDragOff;
    static bool mDragging;

};

};

#endif
