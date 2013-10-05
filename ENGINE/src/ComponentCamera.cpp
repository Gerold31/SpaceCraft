#include "ComponentCamera.hpp"

#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "TypeInfo.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

TypeInfo *ComponentCamera::mType = new TypeInfo("ComponentCamera", &ComponentCamera::createInstance);

ComponentCamera::ComponentCamera(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{
    mCamera = SystemGraphics::getSingleton()->getSceneMgr()->createCamera(object->getName() + "Camera");
    mCamera->lookAt(0,0,-1);
    mCamera->setPosition(0, 0, 0);
    mCamera->setNearClipDistance(0.3);
    mCamera->setFarClipDistance(300.0);
    object->getSceneNode()->attachObject(mCamera);
}

ComponentCamera::~ComponentCamera()
{
}

void *ComponentCamera::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentCamera(object, params);
}

void ComponentCamera::init()
{
}
    
void ComponentCamera::update(float elapsedTime)
{
}

void ComponentCamera::receiveMessage(Message *message)
{
}
