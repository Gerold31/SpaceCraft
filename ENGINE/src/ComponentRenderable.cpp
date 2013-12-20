#include "ComponentRenderable.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "SystemGraphics.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

TypeInfo *ComponentRenderable::mType = new TypeInfo("ComponentRenderable", &ComponentRenderable::createInstance);

ComponentRenderable::ComponentRenderable(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    mEntity = nullptr;
}

ComponentRenderable::~ComponentRenderable()
{
}

void *ComponentRenderable::createInstance(Object *object, ParamMap &params)
{
    return new ComponentRenderable(object, params);
}

void ComponentRenderable::init()
{
    mEntity = ((SystemGraphics *)SystemGraphics::getSingleton())->getSceneMgr()->createEntity(mObject->getName() + "RenderableEnt", boost::any_cast<std::string>(mParams["MeshName"]));
    mEntity->getUserObjectBindings().setUserAny("Object", Ogre::Any(mObject));
    mObject->getSceneNode()->attachObject(mEntity);
}
    
void ComponentRenderable::update(float elapsedTime)
{
}

void ComponentRenderable::receiveMessage(Message *message)
{
}
