#include "Object.hpp"

#include "Component.hpp"
#include "MessageMove.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

Object::Object(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parentNode, Ogre::String name, Object *parent) :
    MessageReceiver(MessageReceiver::RECEIVER_OBJECT),
    mName(name)
{
    LOG_IN("object");
    mNode = parentNode->createChildSceneNode(name);
    mNode->setPosition(pos);
    mNode->setOrientation(ori);
    mNode->getUserObjectBindings().setUserAny("Object", Ogre::Any(this));
    mInit = mReady = false;
    mParent = parent;
    LOG_OUT("object");
}

Object::~Object()
{
    LOG_IN("object");
    mComponentsMutex.lock();
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        delete (*i);
        (*i) = nullptr;
    }
    mComponents.clear();
    mComponentsMutex.unlock();
    LOG_OUT("object");
}
    
void Object::addComponent(Component *component)
{
    LOG_IN("object");
    mComponentsMutex.lock();
    mComponents.push_back(component);
    mComponentsMutex.unlock();
    LOG_OUT("object");
}

bool Object::init()
{
    LOG_IN("object");
    bool ret = true;
    mComponentsMutex.lock();
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        if(!(*i)->isReady() && !(*i)->init())
            ret = false;
    }
    mComponentsMutex.unlock();
    mInit = ret;
    LOG_OUT("object");
    return ret;
}

void Object::update(float elapsedTime)
{
    LOG_IN_FRAME;
    if(!mReady)
    {
        LOG_OUT_FRAME;
        return;
    }
    if(!mInit && !init())
    {
        LOG_OUT_FRAME;
        return;
    }

    mComponentsMutex.lock();
    for(size_t i = 0; i<mComponents.size(); i++)
    {
        mComponents.at(i)->update(elapsedTime);
    }
    mComponentsMutex.unlock();
    LOG_OUT_FRAME;
}

void Object::receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageSetPosition::getID())
    {
        MessageSetPosition *m = (MessageSetPosition *)message;
        mNode->setPosition(m->mX, m->mY, m->mZ);
    }
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->receiveMessage(message);
    }
    LOG_OUT_MSG;
}

Component *Object::getComponent(TypeInfo *type, size_t i)
{
    LOG_IN("object");
    mComponentsMutex.lock();
    for(std::vector<Component *>::iterator j = mComponents.begin(); j!=mComponents.end(); ++j)
    {
        if((*j)->getType() == type)
        {
            if(i == 0)
            {
                mComponentsMutex.unlock();
                LOG_OUT("object");
                return *j;
            }
            i--;
        }
    }
    mComponentsMutex.unlock();
    LOG_OUT("object");
    return nullptr;
}

void Object::ready()
{
    LOG_IN("object");
    mReady = true;
    LOG_OUT("object");
}

void Object::removeChild(Object *obj)
{
    LOG_IN("object");
    for(std::vector<Object *>::iterator i=mChilds.begin(); i!=mChilds.end(); ++i)
    {
        if(*i == obj)
        {
            mChilds.erase(i);
            break;
        }
    }
    LOG_OUT("object");
}
