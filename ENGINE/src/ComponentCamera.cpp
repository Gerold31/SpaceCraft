#include "ComponentCamera.hpp"

#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "TypeInfo.hpp"
#include "MessageMove.hpp"
#include "ComponentMover.hpp"
#include "SystemLog.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

TypeInfo *ComponentCamera::mType = new TypeInfo("ComponentCamera", &ComponentCamera::createInstance);

ComponentCamera::ComponentCamera(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mCamera = nullptr;
    LOG_OUT("component");
}

ComponentCamera::~ComponentCamera()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentCamera::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentCamera(object, params);
}

bool ComponentCamera::init()
{
    LOG_IN("component");
    mCamera = SystemGraphics::getSingleton()->getSceneMgr()->createCamera(mObject->getName() + "Camera");
    mCamera->lookAt(0,0,-1);
    mCamera->setPosition(0, 0, 0);
    mCamera->setNearClipDistance(0.3);
    mCamera->setFarClipDistance(300.0);

    for(int i=0; i<mObject->getNumberComponents(); i++)
    {
        Component *c = mObject->getComponent(i);
        if(c->getType() == ComponentMover::getType())
        {
            ((ComponentMover *)c)->getRollNode()->attachObject(mCamera);
            mReady = true;
            LOG_OUT("component");
            return true;
        }
    }
    mObject->getSceneNode()->attachObject(mCamera);
    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentCamera::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentCamera::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    LOG_OUT_MSG;
}
