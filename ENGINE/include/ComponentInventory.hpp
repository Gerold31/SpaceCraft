#ifndef _COMPONENTINVENTORY_HPP_
#define _COMPONENTINVENTORY_HPP_

#include "ComponentItemContainer.hpp"

namespace ENGINE
{

class ComponentInventory : public ComponentItemContainer
{
public:
    ComponentInventory(Object *object, ParamMap &params);
    ~ComponentInventory();

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
