#include "Object.hpp"

#include "Component.hpp"
#include "MessageMove.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

Object::Object(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name) :
    MessageReceiver(MessageReceiver::RECEIVER_OBJECT),
    mName(name)
{
    LOG_IN("object");
    mNode = parent->createChildSceneNode(name);
    mNode->setPosition(pos);
    mNode->setOrientation(ori);
    mNode->getUserObjectBindings().setUserAny("Object", Ogre::Any(this));
    mInit = mReady = false;
    LOG_OUT("object");
}

Object::~Object()
{
    LOG_IN("object");
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        delete (*i);
        (*i) = nullptr;
    }
    mComponents.clear();
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

void Object::init()
{
    LOG_IN("object");
    mComponentsMutex.lock();
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->init();
    }
    mComponentsMutex.unlock();
    LOG_OUT("object");
}

void Object::update(float elapsedTime)
{
    if(!mReady)
        return;
    if(!mInit)
    {
        mInit = true;
        init();
    }

    mComponentsMutex.lock();
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->update(elapsedTime);
    }
    mComponentsMutex.unlock();
}

void Object::receiveMessage(Message *message)
{
    if(message->getID() == MessageSetPosition::getID())
    {
        MessageSetPosition *m = (MessageSetPosition *)message;
        mNode->setPosition(m->mX, m->mY, m->mZ);
    }
    mComponentsMutex.lock();
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->receiveMessage(message);
    }
    mComponentsMutex.unlock();
}

void Object::ready()
{
    LOG_IN("object");
    mReady = true;
    LOG_OUT("object");
}
