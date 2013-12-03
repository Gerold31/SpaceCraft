#include "Object.hpp"

#include "Component.hpp"

using namespace ENGINE;

Object::Object(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name) :
    MessageReceiver(MessageReceiver::RECEIVER_OBJECT),
    mName(name)
{
    mNode = parent->createChildSceneNode(name);
    mNode->setPosition(pos);
    mNode->setOrientation(ori);
    mNode->getUserObjectBindings().setUserAny("Object", Ogre::Any(this));
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
    mComponents.push_back(component);
}

void Object::init()
{
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->init();
    }
}

void Object::update(float elapsedTime)
{
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->update(elapsedTime);
    }
}

void Object::receiveMessage(Message *message)
{
    for(std::vector<Component *>::iterator i = mComponents.begin(); i!=mComponents.end(); ++i)
    {
        (*i)->receiveMessage(message);
    }
}