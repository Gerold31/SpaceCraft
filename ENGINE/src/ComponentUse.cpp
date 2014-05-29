#include "ComponentUse.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "SystemGraphics.hpp"
#include "MessageUse.hpp"
#include "ComponentCamera.hpp"
#include "Object.hpp"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRay.h>

using namespace ENGINE;

TypeInfo *ComponentUse::mType = new TypeInfo("ComponentUse", &ComponentUse::createInstance);

ComponentUse::ComponentUse(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mParent = nullptr;
    LOG_OUT("component");
}

ComponentUse::~ComponentUse()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentUse::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentUse(object, params);
}

bool ComponentUse::init()
{
    LOG_IN("component");
    for(int i=0; i<mObject->getNumberComponents(); i++)
    {
        Component *c = mObject->getComponent(i);
        if(c->getType() == ComponentCamera::getType())
        {
            mParent = (ComponentCamera *)c;
            break;
        }
    }
    if(!mParent)
        LOG("No ComponentCamera found!", "error");
    
    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentUse::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentUse::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageUse::getID())
    {
        Ogre::Ray ray(mParent->getCamera()->getDerivedPosition(), mParent->getCamera()->getDerivedDirection());
        Ogre::RaySceneQuery *raySceneQuery = SystemGraphics::getSingleton()->getSceneMgr()->createRayQuery(ray);
        raySceneQuery->setRay(ray);
        raySceneQuery->setSortByDistance(true);

        Ogre::RaySceneQueryResult &result = raySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = result.begin();

        while(i!=result.end())
        {
            if(i->distance > 10)
                break;

            if(i->movable && i->movable->getMovableType() == "Entity")
            {
                Object *obj = Ogre::any_cast<Object *>(i->movable->getUserObjectBindings().getUserAny("Object"));
                if(obj && obj != mObject)
                {
                    LOG("use " + obj->getName(), "component");
                    MessageUse msg(mObject->getName());
                    msg.sendTo(obj);
                    break;
                }
            }
            ++i;
        }
        SystemGraphics::getSingleton()->getSceneMgr()->destroyQuery(raySceneQuery);
    }
    LOG_OUT_MSG;
}
