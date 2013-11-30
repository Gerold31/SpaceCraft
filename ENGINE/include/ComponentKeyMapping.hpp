#ifndef _COMPONENTKEYMAPPING_HPP_
#define _COMPONENTKEYMAPPING_HPP_

#include "Component.hpp"

namespace ENGINE
{

class ComponentKeyMapping : public Component
{
public:
    ComponentKeyMapping(Object *object, ParamMap &params);
    ~ComponentKeyMapping();

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