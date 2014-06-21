#ifndef _COMPONENTHOTBAR_HPP_
#define _COMPONENTHOTBAR_HPP_

#include "ComponentItemContainer.hpp"

namespace ENGINE
{

class ComponentHotbar : public ComponentItemContainer
{
public:
    ComponentHotbar(Object *object, ParamMap &params);
    ~ComponentHotbar();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

};

};

#endif
