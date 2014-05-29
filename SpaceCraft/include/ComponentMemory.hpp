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

    bool init();
    void update(float elapsedTime);

    WORD *getMemory() {return mMem;}

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    WORD mMem[1<<16];
};

};

#endif
