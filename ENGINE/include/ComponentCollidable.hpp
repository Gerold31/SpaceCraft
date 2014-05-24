#ifndef _COMPONENTCOLLIDABLE_HPP_
#define _COMPONENTCOLLIDABLE_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentCollidable : public Component
{
public:
    ComponentCollidable(Object *object, ParamMap &params);
    ~ComponentCollidable();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

};

};

#endif