#include "ComponentViewport.hpp"

#include "ComponentCamera.hpp"
#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "SystemGUI.hpp"

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

bool ComponentViewport::init()
{
    LOG_IN("component");
    Ogre::Camera *cam = nullptr;
    for(size_t i=0; i<mObject->getNumberComponents(); i++)
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
        unsigned short i = SystemGraphics::getSingleton()->getWindow()->getNumViewports();
        mViewport = SystemGraphics::getSingleton()->getWindow()->addViewport(cam, 100, 0, 0, 1, 1);

        mViewport->setAutoUpdated(true);
        mViewport->setBackgroundColour(Ogre::ColourValue(0.5,0,0));
        mViewport->setOverlaysEnabled(true);

        cam->setAspectRatio(1.0f * mViewport->getActualWidth() / mViewport->getActualHeight());
        
        SystemGUI::getSingleton()->setActiveViewport(i);

    }else
    {
        LOG_OUT("component");
        throw "ComponentViewport requires a ComponentCamera";
    }

    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentViewport::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentViewport::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    SystemGUI::getSingleton()->receiveMessage(message);
    LOG_OUT_MSG;
}
