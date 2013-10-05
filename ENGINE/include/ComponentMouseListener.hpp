#ifndef _COMPONENTMOUSELISTENER_HPP_
#define _COMPONENTMOUSELISTENER_HPP_

#include <string>
#include "Component.hpp"

namespace ENGINE
{

class ComponentMouseListener : public Component
{
public:
    ComponentMouseListener(Object *object, std::map<std::string, std::string> params);
    ~ComponentMouseListener();

    static void *createInstance(Object *object, std::map<std::string, std::string> params);
    
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

};

};

#endif
