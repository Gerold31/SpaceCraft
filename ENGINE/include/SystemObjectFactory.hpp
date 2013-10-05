#ifndef _SYSTEMOBJECTFACTORY_HPP_
#define _SYSTEMOBJECTFACTORY_HPP_

#include "System.hpp"

#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreString.h"

#include <vector>
#include <map>
#include <string>

namespace ENGINE
{

class Component;
class Object;
class TypeInfo;

typedef std::map<std::string, std::string> ParameterList;
typedef std::pair<TypeInfo *, ParameterList> ComponentListElement;
typedef std::vector<ComponentListElement> ComponentList;
typedef std::map<std::string, ComponentList> ObjectMap;

class SystemObjectFactory : public System, public Singleton<SystemObjectFactory>
{
friend class Singleton<SystemObjectFactory>;
public:    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    void registerComponent(TypeInfo *type);

    Object *createObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, std::string type = "");

private:
    SystemObjectFactory();
    SystemObjectFactory(const SystemObjectFactory &) {}
    ~SystemObjectFactory();

    std::vector<Object *> mObjects;
    std::vector<Component *> mComponents;
    ObjectMap mObjectMap;
    std::map<std::string, TypeInfo *> mComponentMap;
    
};

};

#endif
