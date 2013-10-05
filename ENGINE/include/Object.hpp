#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "MessageReceiver.hpp"

#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreString.h"

#include <vector>

namespace ENGINE
{
    
class Component;
class Message;

class Object : public MessageReceiver
{
public:
    Object(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name);
    ~Object();
    
    /**
        \note Ownership of the component goes to the object
     */
    void addComponent(Component *component);
    int getNumberComponents() {return mComponents.size();}
    Component *getComponent(int i) {return mComponents.at(i);}

    void update(float elapsedTime);
    void receiveMessage(Message *message);

    Ogre::SceneNode *getSceneNode() {return mNode;}
    Ogre::String getName() {return mName;}

private:
    Ogre::SceneNode *mNode;
    Ogre::String mName;
    std::vector<Component *> mComponents;

};

};

#endif
