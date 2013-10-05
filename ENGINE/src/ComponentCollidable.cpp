#include "ComponentCollidable.hpp"

#include "TypeInfo.hpp"

using namespace ENGINE;

TypeInfo *ComponentCollidable::mType = new TypeInfo("ComponentCollidable", &ComponentCollidable::createInstance);

ComponentCollidable::ComponentCollidable(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{

}

ComponentCollidable::~ComponentCollidable()
{
}

void *ComponentCollidable::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentCollidable(object, params);
}
    
void ComponentCollidable::update(float elapsedTime)
{
}

void ComponentCollidable::receiveMessage(Message *message)
{
}
