#include "SystemObjectFactory.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "Component.hpp"
#include "MessageObjectFactory.hpp"
#include "SystemLog.hpp"

#include <fstream>

using namespace ENGINE;

SystemObjectFactory::SystemObjectFactory() :
    System("SystemObjectFactory")
{
    LOG_IN("system");
    LOG_OUT("system");
}

SystemObjectFactory::~SystemObjectFactory()
{
    LOG_IN("system");
    LOG_OUT("system");
}

void SystemObjectFactory::init()
{
    LOG_IN("system");
    mObjectMap[""] = ComponentList();

    std::fstream file;

    file.open("objects.cfg");

    if(file.eof() || !file.is_open())
    {
        file.close();
        LOG_OUT("system");
        return;
    }

    std::string line;
    std::getline(file, line);

    while(!file.eof())
    {
        std::string name = line;

        ComponentList entityList;

        LOG("add Object " + name, "log");
        
        std::getline(file, line);

        while(!file.eof())
        {
            if(line[0] != '\t')
                break;

            ComponentListElement element;
            ParamMap params;

            line = line.substr(1);

            if(mComponentMap.find(line) == mComponentMap.end())
                throw "no \"" + line + "\" found";
            
            LOG("\tadd Component " + line, "log");
            
            element.first = mComponentMap[line];

            while(!file.eof())
            {
                std::getline(file, line);
                if(line[0] != '\t' || line[1] != '\t')
                    break;

                line = line.substr(2);
                line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
                std::string param, value;
                param = line.substr(0, line.find_first_of('='));
                value = line.substr(line.find_first_of('=')+1);
                LOG("\t\tadd Param " + param + ", value = " + value, "log");
                params[param] = value;
            }
            element.second = params;
            entityList.push_back(element);
        }
        mObjectMap[name] = entityList;
    }

    file.close();
    LOG_OUT("system");
}

void SystemObjectFactory::update(float elapsedTime)
{
    LOG_IN_FRAME;
    mComponentsMutex.lock();
    for(auto i = mObjects.begin(); i!=mObjects.end(); ++i)
    {
        (*i)->update(elapsedTime);
    }
    mComponentsMutex.unlock();
    LOG_OUT_FRAME;
}

void SystemObjectFactory::receiveMessage(Message *msg)
{
    LOG_IN_FRAME;
    // @todo destroyObject
    if(msg->getID() == MessageCreateObject::getID())
    {
        MessageCreateObject *m = (MessageCreateObject *)msg;
        createObject(m->mPos, m->mOri, m->mParent, m->mName, m->mType);
    }
    LOG_OUT_FRAME;
}

void SystemObjectFactory::registerComponent(TypeInfo *type)
{
    LOG_IN("system");
    LOG("register " + type->getName(), "log");
    mComponentMap[type->getName()] = type;
    LOG_OUT("system");
}

Object *SystemObjectFactory::createObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, std::string type)
{
    LOG_IN("system");
    LOG("add Object:", "log");
    LOG("\tType: " + type, "log");
    LOG("\tName: " + name, "log");
    ObjectMap::iterator components = mObjectMap.find(type);
    if(components == mObjectMap.end())
    {
        LOG_OUT("system");
        return nullptr;
    }
    Object *object = new Object(pos, ori, parent, name);
    for(ComponentList::iterator i = components->second.begin(); i!= components->second.end(); ++i)
    {
        LOG("\tadd Component " + (*i).first->getName(), "log");
        Component *component = (Component *)(*i).first->createInstance(object, (*i).second);
        object->addComponent(component);
        mComponents.push_back(component);
    }
    mComponentsMutex.lock();
    mObjects.push_back(object);
    mComponentsMutex.unlock();
    object->ready();
    LOG_OUT("system");
    return object;
}

Object *SystemObjectFactory::getObject(std::string name)
{
    LOG_IN("system");
    mComponentsMutex.lock();
    for(auto i = mObjects.begin(); i!=mObjects.end(); ++i)
    {
        if((*i)->getName() == name)
        {
            mComponentsMutex.unlock();
            LOG_OUT("system");
            return *i;
        }
    }
    mComponentsMutex.unlock();
    LOG_OUT("system");
    return nullptr;
}

Component *SystemObjectFactory::createComponent(Object *parent, std::string name, ParamMap &params)
{
    LOG_IN("system");
    LOG("add Component " + name, "log");
    assert(mComponentMap.count(name) > 0);
    Component *component = (Component *)mComponentMap[name]->createInstance(parent, params);
    parent->addComponent(component);
    mComponents.push_back(component);

    LOG_OUT("system");
    return component;
}
