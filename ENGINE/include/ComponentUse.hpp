#ifndef _COMPONENTUSE_HPP_
#define _COMPONENTUSE_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentCamera;

class ComponentUse : public Component
{
public:
    ComponentUse(Object *object, ParamMap &params);
    ~ComponentUse();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    ComponentCamera *mParent;

};

};

#endif
