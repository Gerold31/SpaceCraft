#include "Object.hpp"

#include "Component.hpp"
#include "MessageMove.hpp"

using namespace ENGINE;

Object::Object(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name) :
    MessageReceiver(MessageReceiver::RECEIVER_OBJECT),
    mName(name)
{
    mNode = parent->createChildSceneNode(name);
    mNode->setPosition(pos);
    mNode->setOrientation(ori);
    mNode->getUserObjectBindings().setUserAny("Object", Ogre::Any(this));
    mInit = false;
}

Object::~Object()
{
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        delete (*i);
        (*i) = nullptr;
    }
    mComponents.clear();
}
    
void Object::addComponent(Component *component)
{
    mComponentsMutex.lock();
    mComponents.push_back(component);
    mComponentsMutex.unlock();
}

void Object::init()
{
    mComponentsMutex.lock();
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->init();
    }
    mComponentsMutex.unlock();
}

void Object::update(float elapsedTime)
{
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
