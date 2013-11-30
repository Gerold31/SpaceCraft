#ifndef _COMPONENTJOYSTICKLISTENER_HPP_
#define _COMPONENTJOYSTICKLISTENER_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentJoystickListener : public Component
{
public:
    ComponentJoystickListener(Object *object, ParamMap &params);
    ~ComponentJoystickListener();

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
