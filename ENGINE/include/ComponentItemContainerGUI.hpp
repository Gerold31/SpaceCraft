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
    void notifyStartDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result);
    void notifyRequestDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool &result);
    void notifyEndDrop(MyGUI::DDContainer *sender, const MyGUI::DDItemInfo &info, bool result);
    void notifyDropState(MyGUI::DDContainer *sender, MyGUI::DDItemState state);
    void notifyNotifyItem(MyGUI::ItemBox *sender, const MyGUI::IBNotifyItemData &info);

    void requestCreateWidgetItem(MyGUI::ItemBox *sender, MyGUI::Widget *item);
    void requestDrawItem(MyGUI::ItemBox *sender, MyGUI::Widget *item, const MyGUI::IBDrawItemInfo &info);

    ComponentItemContainer *mContainer;

    MyGUI::VectorWidgetPtr mLayoutRoot;
	MyGUI::ItemBox *mItemBox;
    MyGUI::Widget *mDragItem;
    MyGUI::IntPoint mDragOff;
    bool mDragging;

};

};

#endif
