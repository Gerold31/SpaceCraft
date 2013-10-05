#ifndef _COMPONENTJOYSTICKLISTENER_HPP_
#define _COMPONENTJOYSTICKLISTENER_HPP_

#include <string>
#include "Component.hpp"

namespace ENGINE
{

class ComponentJoystickListener : public Component
{
public:
    ComponentJoystickListener(Object *object, std::map<std::string, std::string> params);
    ~ComponentJoystickListener();

    static void *createInstance(Object *object, std::map<std::string, std::string> params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

};

};

#endif
