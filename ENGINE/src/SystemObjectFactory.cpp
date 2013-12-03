#include "SystemObjectFactory.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "Component.hpp"

#include <fstream>

using namespace ENGINE;

SystemObjectFactory::SystemObjectFactory() :
    System("SystemObjectFactory")
{
}

SystemObjectFactory::~SystemObjectFactory()
{
}

void SystemObjectFactory::init()
{
    mObjectMap[""] = ComponentList();

    printf("SystemObjectFactory::init\n");

    std::fstream file;

    file.open("objects.cfg");

    if(file.eof() || !file.is_open())
    {
        file.close();
        return;
    }

    std::string line;
    std::getline(file, line);

    while(!file.eof())
    {
        std::string name = line;

        ComponentList entityList;

        printf("\tadd Object \"%s\"\n", name.c_str());
        
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
            
            printf("\t\tadd Component \"%s\"\n", line.c_str());
            
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
                printf("\t\t\tadd Param \"%s\", Value \"%s\"\n", param.c_str(), value.c_str());
                params[param] = value;
            }
            element.second = params;
            entityList.push_back(element);
        }
        mObjectMap[name] = entityList;
    }

    printf("finished\n");

    file.close();
}

void SystemObjectFactory::update(float elapsedTime)
{
    for(std::vector<Object *>::iterator i = mObjects.begin(); i!=mObjects.end(); ++i)
    {
        (*i)->update(elapsedTime);
    }
}

void SystemObjectFactory::receiveMessage(Message *msg)
{
}

void SystemObjectFactory::registerComponent(TypeInfo *type)
{
    printf("register %s\n", type->getName().c_str());
    mComponentMap[type->getName()] = type;
}

Object *SystemObjectFactory::createObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, std::string type)
{
    printf("add Object:\n\tType: %s\n\tName: %s\n", type.c_str(), name.c_str());
    ObjectMap::iterator components = mObjectMap.find(type);
    if(components == mObjectMap.end())
        return nullptr;
    Object *object = new Object(pos, ori, parent, name);
    for(ComponentList::iterator i = components->second.begin(); i!= components->second.end(); ++i)
    {
        printf("add Component %s\n", (*i).first->getName().c_str());
        Component *component = (Component *)(*i).first->createInstance(object, (*i).second);
        object->addComponent(component);
        mComponents.push_back(component);
    }
    mObjects.push_back(object);
    object->init();
    return object;
}

Object *SystemObjectFactory::getObject(std::string name)
{
    for(std::vector<Object *>::iterator i = mObjects.begin(); i!=mObjects.end(); ++i)
    {
        if((*i)->getName() == name)
            return *i;
    }
    return nullptr;
}

Component *SystemObjectFactory::createComponent(Object *parent, std::string name, ParamMap &params)
{
    printf("add Component %s\n", name.c_str());
    assert(mComponentMap.count(name) > 0);
    Component *component = (Component *)mComponentMap[name]->createInstance(parent, params);
    parent->addComponent(component);
    mComponents.push_back(component);
    component->init();
    return component;
}
