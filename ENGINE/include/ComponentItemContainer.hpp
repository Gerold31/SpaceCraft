#ifndef _COMPONENTITEMCONTAINER_HPP_
#define _COMPONENTITEMCONTAINER_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentItem;

class ComponentItemContainer : public Component
{
public:
    ComponentItemContainer(Object *object, ParamMap &params, TypeInfo *type);
    ~ComponentItemContainer();
    
    bool init();
    void update(float elapsedTime);
	
    void addItem(ComponentItem *item, size_t slot);
    void removeItem(size_t slot);
    size_t getNumberItems() {return mNumberItems;}
    ComponentItem *getItem(size_t slot) { if(slot<mNumberItems) return mItems[slot]; return nullptr; }

protected:
    void _receiveMessage(Message *message);

private:
    ComponentItem **mItems;
    size_t mNumberItems;
    bool mFirstTry;

};

};

#endif
