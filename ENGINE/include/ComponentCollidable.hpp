#ifndef _COMPONENTCOLLIDABLE_HPP_
#define _COMPONENTCOLLIDABLE_HPP_

#include <string>
#include "Component.hpp"

namespace ENGINE
{

class ComponentCollidable : public Component
{
public:
    ComponentCollidable(Object *object, std::map<std::string, std::string> params);
    ~ComponentCollidable();

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