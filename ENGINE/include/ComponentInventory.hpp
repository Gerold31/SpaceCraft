#ifndef _COMPONENTINVENTORY_HPP_
#define _COMPONENTINVENTORY_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentItem;

class ComponentInventory : public Component
{
public:
    ComponentInventory(Object *object, ParamMap &params);
    ~ComponentInventory();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}
	
    void addItem(ComponentItem *item, size_t slot);
    void removeItem(size_t slot);
    size_t getNumberItems() {return mNumberItems;}
    ComponentItem *getItem(size_t slot) { if(slot<mNumberItems) return mItems[slot]; return nullptr; }

private:
    static TypeInfo *mType;

    ComponentItem **mItems;
    size_t mNumberItems;

};

};

#endif
