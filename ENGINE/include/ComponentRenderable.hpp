#ifndef _COMPONENTRENDERABLE_HPP_
#define _COMPONENTRENDERABLE_HPP_

#include "Component.hpp"

#include "OGRE/OgreEntity.h"

namespace ENGINE
{

class ComponentRenderable : public Component
{
public:
    ComponentRenderable(Object *object, ParamMap &params);
    ~ComponentRenderable();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);

    void setMaterial(const Ogre::MaterialPtr &material);
    
    static TypeInfo *getType() {return mType;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    Ogre::Entity* mEntity;

};

};

#endif
