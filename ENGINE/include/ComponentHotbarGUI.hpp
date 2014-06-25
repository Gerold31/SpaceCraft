#ifndef _COMPONENTHOTBARGUI_HPP_
#define _COMPONENTHOTBARGUI_HPP_

#include "ComponentItemContainerGUI.hpp"

namespace ENGINE
{

class ComponentHotbarGUI : public ComponentItemContainerGUI
{
public:
    ComponentHotbarGUI(Object *object, ParamMap &params);
    ~ComponentHotbarGUI();

    static void *createInstance(Object *object, ParamMap &params);

    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    void selectIndex(int index);
    static TypeInfo *mType;

    bool mEnable;
    int mIndex;

};

};

#endif
