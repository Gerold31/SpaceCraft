#ifndef _COMPONENTMULTIUSE_HPP_
#define _COMPONENTMULTIUSE_HPP_

#include "Component.hpp"

#include "MyGUI.h"

namespace ENGINE
{

class ComponentMultiUse : public Component
{
public:
    ComponentMultiUse(Object *object, ParamMap &params);
    ~ComponentMultiUse();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    StateMap getState();

    static TypeInfo *getType() {return mType;}

private:
    void eventListClick(MyGUI::ListBox* sender, size_t index);

    static TypeInfo *mType;

    MyGUI::VectorWidgetPtr mUseMenu;
    MyGUI::ListBox *mUseList;
    std::vector<std::string> mOptions;

    std::string mUser;

};

};

#endif
