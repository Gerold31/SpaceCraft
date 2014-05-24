#ifndef _SYSTEMOBJECTFACTORY_HPP_
#define _SYSTEMOBJECTFACTORY_HPP_

#include "Defines.hpp"

#include "System.hpp"

#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreString.h"

#include <vector>

#include <boost/thread/mutex.hpp>

namespace ENGINE
{

class Component;
class Object;
class TypeInfo;

typedef std::pair<TypeInfo *, ParamMap> ComponentListElement;
typedef std::vector<ComponentListElement> ComponentList;
typedef std::vector<std::string> ObjectList;
typedef std::map<std::string, std::pair<ComponentList, ObjectList> > ObjectMap;

class SystemObjectFactory : public System, public Singleton<SystemObjectFactory>
{
friend class Singleton<SystemObjectFactory>;
public:    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    void registerComponent(TypeInfo *type);

    Object *getObject(std::string name);
    Component *createComponent(Object *parent, std::string name, ParamMap &params);

private:
    SystemObjectFactory();
    SystemObjectFactory(const SystemObjectFactory &) : System("SystemObjectFactory") {}
    ~SystemObjectFactory();

    Object *createObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parentNode, Ogre::String name, std::string type = "", Object *parent = nullptr);

    std::string parseObject(std::fstream &file, std::string name, int depth);

    std::vector<Object *> mObjects;
    std::vector<Component *> mComponents;
    ObjectMap mObjectMap;
    std::map<std::string, TypeInfo *> mComponentMap;

    boost::recursive_mutex mObjectsMutex;
    
};

};

#endif
