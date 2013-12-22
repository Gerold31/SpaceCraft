#ifndef _COMPONENTMEMORY_HPP_
#define _COMPONENTMEMORY_HPP_

#include "SpaceCraftDefines.hpp"
#include "Component.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentMemory : public Component
{
public:
    ComponentMemory(Object *object, ParamMap &params);
    ~ComponentMemory();

    static void *createInstance(Object *object, ParamMap &params);

    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    WORD *getMemory() {return mMem;}

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    WORD mMem[1<<16];
};

};

#endif
