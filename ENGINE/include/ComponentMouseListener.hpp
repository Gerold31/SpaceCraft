#ifndef _COMPONENTMOUSELISTENER_HPP_
#define _COMPONENTMOUSELISTENER_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentMouseListener : public Component
{
public:
    ComponentMouseListener(Object *object, ParamMap &params);
    ~ComponentMouseListener();

    static void *createInstance(Object *object, ParamMap &params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

};

};

#endif
