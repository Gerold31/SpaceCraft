#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "MessageReceiver.hpp"

#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreString.h"

#include <boost/thread/mutex.hpp>

#include <vector>

namespace ENGINE
{
    
class Component;
class Message;
class TypeInfo;

class Object : public MessageReceiver
{
public:
    Object(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parentNode, Ogre::String name, Object *parent = nullptr);
    ~Object();
    
    /**
        \note Ownership of the component goes to the object
     */
    void addComponent(Component *component);
    size_t getNumberComponents() {return mComponents.size();}
    Component *getComponent(size_t i) {return mComponents.at(i);}
    Component *getComponent(TypeInfo *type, size_t i = 0);

    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    void ready();

    Ogre::SceneNode *getSceneNode() {return mNode;}
    Ogre::String getName() {return mName;}

    Object *getParent() {return mParent;}
    void addChild(Object *obj) {mChilds.push_back(obj);}
    Object *getChild(size_t i) {return mChilds.at(i);}
    size_t getNumberChilds() {return mChilds.size();}
    void removeChild(Object *obj);

private:
    Ogre::SceneNode *mNode;
    Ogre::String mName;
    Object *mParent;
    std::vector<Object *> mChilds;
    std::vector<Component *> mComponents;
    boost::recursive_mutex mComponentsMutex;
    bool mInit;
    bool mReady;
    bool mEnable;

};

};

#endif
