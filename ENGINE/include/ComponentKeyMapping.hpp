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
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

};

};

#endif