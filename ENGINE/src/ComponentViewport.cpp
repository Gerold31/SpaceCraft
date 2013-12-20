#include "ComponentViewport.hpp"

#include "ComponentCamera.hpp"
#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "TypeInfo.hpp"
#include "SystemLog.hpp"

#include "OGRE/OgreRenderWindow.h"

using namespace ENGINE;

TypeInfo *ComponentViewport::mType = new TypeInfo("ComponentViewport", &ComponentViewport::createInstance);

ComponentViewport::ComponentViewport(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mViewport = nullptr;
    LOG_OUT("component");
}

ComponentViewport::~ComponentViewport()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentViewport::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentViewport(object, params);
}

void ComponentViewport::init()
{
    LOG_IN("component");
    Ogre::Camera *cam = nullptr;
    for(int i=0; i<mObject->getNumberComponents(); i++)
    {
        Component *c = mObject->getComponent(i);
        if(c->getType() == ComponentCamera::getType())
        {
            cam = ((ComponentCamera*)c)->getCamera();
            break;
        }
    }
    if(cam)
    {
        mViewport = SystemGraphics::getSingleton()->getWindow()->addViewport(cam, 100, 0, 0, 1, 1);

        mViewport->setAutoUpdated(true);
        mViewport->setBackgroundColour(Ogre::ColourValue(0.5,0,0));

        cam->setAspectRatio(1.0f * mViewport->getActualWidth() / mViewport->getActualHeight());

    }else
    {
        LOG_OUT("component");
        throw "ComponentViewport requires a ComponentCamera";
    }
    LOG_OUT("component");
}
    
void ComponentViewport::update(float elapsedTime)
{
}

void ComponentViewport::receiveMessage(Message *message)
{
}
