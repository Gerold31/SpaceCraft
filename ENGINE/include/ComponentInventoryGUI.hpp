#ifndef _COMPONENTINVENTORYGUI_HPP_
#define _COMPONENTINVENTORYGUI_HPP_

#include "ComponentItemContainerGUI.hpp"

namespace ENGINE
{

class ComponentInventoryGUI : public ComponentItemContainerGUI
{
public:
    ComponentInventoryGUI(Object *object, ParamMap &params);
    ~ComponentInventoryGUI();

    static void *createInstance(Object *object, ParamMap &params);

    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    bool mEnable;

};

};

#endif
