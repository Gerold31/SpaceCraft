#ifndef _COMPONENTCAMERA_HPP_
#define _COMPONENTCAMERA_HPP_

#include "Component.hpp"

#include "OGRE/OgreCamera.h"

#include <string>

namespace ENGINE
{

class ComponentCamera : public Component
{
public:
    ComponentCamera(Object *object, std::map<std::string, std::string> params);
    ~ComponentCamera();

    static void *createInstance(Object *object, std::map<std::string, std::string> params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

    Ogre::Camera *getCamera() {return mCamera;}

    Ogre::SceneNode *getYawNode() {return mCameraYawNode;}
    Ogre::SceneNode *getPitchNode() {return mCameraPitchNode;}
    Ogre::SceneNode *getRollNode() {return mCameraRollNode;}

private:
    static TypeInfo *mType;

    Ogre::Camera *mCamera;
    Ogre::SceneNode *mCameraYawNode;
    Ogre::SceneNode *mCameraPitchNode;
    Ogre::SceneNode *mCameraRollNode;

};

};

#endif
