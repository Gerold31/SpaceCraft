#include "ComponentCamera.hpp"

#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "TypeInfo.hpp"
#include "MessageMove.hpp"
#include "ComponentMover.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

TypeInfo *ComponentCamera::mType = new TypeInfo("ComponentCamera", &ComponentCamera::createInstance);

ComponentCamera::ComponentCamera(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    mCamera = SystemGraphics::getSingleton()->getSceneMgr()->createCamera(object->getName() + "Camera");
    mCamera->lookAt(0,0,-1);
    mCamera->setPosition(0, 0, 0);
    mCamera->setNearClipDistance(0.3);
    mCamera->setFarClipDistance(300.0);
}

ComponentCamera::~ComponentCamera()
{
}

void *ComponentCamera::createInstance(Object *object, ParamMap &params)
{
    return new ComponentCamera(object, params);
}

void ComponentCamera::init()
{
    for(int i=0; i<mObject->getNumberComponents(); i++)
    {
        Component *c = mObject->getComponent(i);
        if(c->getType() == ComponentMover::getType())
        {
            ((ComponentMover *)c)->getRollNode()->attachObject(mCamera);
            return;
        }
    }
    mObject->getSceneNode()->attachObject(mCamera);
}
    
void ComponentCamera::update(float elapsedTime)
{
}

void ComponentCamera::receiveMessage(Message *message)
{
}
