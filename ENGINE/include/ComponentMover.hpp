#ifndef _COMPONENTMOVER_HPP_
#define _COMPONENTMOVER_HPP_

#include <string>
#include "Component.hpp"

#include "OGRE/OgreVector3.h"

namespace ENGINE
{

class ComponentMover : public Component
{
public:
    ComponentMover(Object *object, std::map<std::string, std::string> params);
    ~ComponentMover();

    static void *createInstance(Object *object, std::map<std::string, std::string> params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

    double mSpeed;
    Ogre::Vector3 mTranslation;

};

};

#endif