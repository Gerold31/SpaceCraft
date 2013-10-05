#ifndef _COMPONENTRENDERABLE_HPP_
#define _COMPONENTRENDERABLE_HPP_

#include "Component.hpp"

#include "OGRE/OgreEntity.h"

#include <string>

namespace ENGINE
{

class ComponentRenderable : public Component
{
public:
    ComponentRenderable(Object *object, std::map<std::string, std::string> params);
    ~ComponentRenderable();

    static void *createInstance(Object *object, std::map<std::string, std::string> params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);
    
    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    Ogre::Entity* mEntity;

};

};

#endif