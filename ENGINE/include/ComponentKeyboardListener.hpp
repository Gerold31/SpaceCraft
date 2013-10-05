#ifndef _COMPONENTKEYBOARDLISTENER_HPP_
#define _COMPONENTKEYBOARDLISTENER_HPP_

#include <string>
#include "Component.hpp"

namespace ENGINE
{

class ComponentKeyboardListener : public Component
{
public:
    ComponentKeyboardListener(Object *object, std::map<std::string, std::string> params);
    ~ComponentKeyboardListener();

    static void *createInstance(Object *object, std::map<std::string, std::string> params);
    
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

};

};

#endif
