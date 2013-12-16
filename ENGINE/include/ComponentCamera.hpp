#ifndef _COMPONENTCAMERA_HPP_
#define _COMPONENTCAMERA_HPP_

#include "Component.hpp"

#include "OGRE/OgreCamera.h"

namespace ENGINE
{

class ComponentCamera : public Component
{
public:
    ComponentCamera(Object *object, ParamMap &params);
    ~ComponentCamera();

    static void *createInstance(Object *object, ParamMap &params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

    Ogre::Camera *getCamera() {return mCamera;}

private:
    static TypeInfo *mType;

    Ogre::Camera *mCamera;

};

};

#endif
