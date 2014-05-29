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
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

    Ogre::Camera *getCamera() {return mCamera;}

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;

    Ogre::Camera *mCamera;

};

};

#endif
