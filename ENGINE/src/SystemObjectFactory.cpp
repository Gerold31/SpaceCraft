#include "SystemObjectFactory.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"

#include <fstream>

using namespace ENGINE;

SystemObjectFactory::SystemObjectFactory() :
    System()
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

    if(file.eof())
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

        while(!file.eof()){
            std::getline(file, line);
            if(line[0] != '\t')
                break;

            ComponentListElement element;
            ParameterList params;

            line = line.substr(1);

            if(mComponentMap.find(line) == mComponentMap.end())
                throw "no \"" + line + "\" found";

            element.first = mComponentMap[line];
            element.second = params;
            entityList.push_back(element);
        }

        printf("\tadd Object \"%s\"\n", name.c_str());
        mObjectMap[name] = entityList;

        //std::getline(file, line);
    }

    printf("finished\n");

    file.close();
}

void SystemObjectFactory::update(float elapsedTime)
{
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
    return object;
}
