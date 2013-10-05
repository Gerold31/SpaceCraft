#include "ComponentViewport.hpp"

#include "ComponentCamera.hpp"
#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "TypeInfo.hpp"

#include "OGRE/OgreRenderWindow.h"

using namespace ENGINE;

TypeInfo *ComponentViewport::mType = new TypeInfo("ComponentViewport", &ComponentViewport::createInstance);

ComponentViewport::ComponentViewport(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{
    Ogre::Camera *cam = nullptr;
    for(int i=0; i<object->getNumberComponents(); i++)
    {
        Component *c = object->getComponent(i);
        printf("ComponentCamera::getType()->getName() == %s\n", ComponentCamera::getType()->getName().c_str());
        printf("c->getType()->getName() == %s\n", c->getType()->getName().c_str());
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
        throw "ComponentViewport requires a ComponentCamera";
    }
}

ComponentViewport::~ComponentViewport()
{
}

void *ComponentViewport::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentViewport(object, params);
}
    
void ComponentViewport::update(float elapsedTime)
{
}

void ComponentViewport::receiveMessage(Message *message)
{
}
