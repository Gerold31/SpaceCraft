#ifndef _COMPONENTVIEWPORT_HPP_
#define _COMPONENTVIEWPORT_HPP_

#include "Component.hpp"

#include "OGRE/OgreViewport.h"

#include <string>

namespace ENGINE
{

class ComponentViewport : public Component
{
public:
    ComponentViewport(Object *object, std::map<std::string, std::string> params);
    ~ComponentViewport();

    static void *createInstance(Object *object, std::map<std::string, std::string> params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    Ogre::Viewport *mViewport;

};

};

#endif