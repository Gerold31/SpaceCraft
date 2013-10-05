#include "ComponentRenderable.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "SystemGraphics.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

TypeInfo *ComponentRenderable::mType = new TypeInfo("ComponentRenderable", &ComponentRenderable::createInstance);

ComponentRenderable::ComponentRenderable(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{
    mEntity = ((SystemGraphics *)SystemGraphics::getSingleton())->getSceneMgr()->createEntity(mObject->getName() + "RenderableEnt", params["MeshName"]);
    mEntity->getUserObjectBindings().setUserAny("Object", Ogre::Any(mObject));
    mObject->getSceneNode()->attachObject(mEntity);
}

ComponentRenderable::~ComponentRenderable()
{
}

void *ComponentRenderable::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentRenderable(object, params);
}
    
void ComponentRenderable::update(float elapsedTime)
{
}

void ComponentRenderable::receiveMessage(Message *message)
{
}
