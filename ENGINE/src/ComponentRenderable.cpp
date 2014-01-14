#include "ComponentRenderable.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "SystemGraphics.hpp"
#include "SystemLog.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

TypeInfo *ComponentRenderable::mType = new TypeInfo("ComponentRenderable", &ComponentRenderable::createInstance);

ComponentRenderable::ComponentRenderable(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mEntity = nullptr;
    LOG_OUT("component");
}

ComponentRenderable::~ComponentRenderable()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentRenderable::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentRenderable(object, params);
}

void ComponentRenderable::init()
{
    LOG_IN("component");
    mEntity = ((SystemGraphics *)SystemGraphics::getSingleton())->getSceneMgr()->createEntity(mObject->getName() + "RenderableEnt", boost::any_cast<std::string>(mParams["MeshName"]));
    mEntity->getUserObjectBindings().setUserAny("Object", Ogre::Any(mObject));
    SystemGraphics::getSingleton()->getSceneMgr()->getSceneNode(mObject->getName() + boost::any_cast<std::string>(mParams["NodeName"]))->attachObject(mEntity);
    LOG_OUT("component");
}
    
void ComponentRenderable::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentRenderable::receiveMessage(Message *message)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentRenderable::setMaterial(const Ogre::MaterialPtr &material)
{
    LOG_IN("component");
    if(mEntity)
        mEntity->setMaterial(material);
    else
        LOG("mEntity == nulptr", "error");
    LOG_OUT("component");
}
