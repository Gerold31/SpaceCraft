#ifndef _COMPONENTVIEWPORT_HPP_
#define _COMPONENTVIEWPORT_HPP_

#include "Component.hpp"

#include "OGRE/OgreViewport.h"

namespace ENGINE
{

class ComponentViewport : public Component
{
public:
    ComponentViewport(Object *object, ParamMap &params);
    ~ComponentViewport();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    Ogre::Viewport *mViewport;

};

};

#endif