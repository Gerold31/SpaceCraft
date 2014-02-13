#ifndef _COMPONENTKEYBOARDLISTENER_HPP_
#define _COMPONENTKEYBOARDLISTENER_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentKeyboardListener : public Component
{
public:
    ComponentKeyboardListener(Object *object, ParamMap &params);
    ~ComponentKeyboardListener();

    static void *createInstance(Object *object, ParamMap &params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    void enable(bool enable) {mEnabled = enable;}
    bool isEnabled() {return mEnabled;}

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    bool mEnabled;

};

};

#endif
