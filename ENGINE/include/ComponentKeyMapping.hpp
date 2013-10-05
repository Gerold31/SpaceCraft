#ifndef _COMPONENTKEYMAPPING_HPP_
#define _COMPONENTKEYMAPPING_HPP_

#include <string>
#include "Component.hpp"

namespace ENGINE
{

class ComponentKeyMapping : public Component
{
public:
    ComponentKeyMapping(Object *object, std::map<std::string, std::string> params);
    ~ComponentKeyMapping();

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