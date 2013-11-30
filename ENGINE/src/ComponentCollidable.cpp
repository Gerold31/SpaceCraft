#include "ComponentCollidable.hpp"

#include "TypeInfo.hpp"

using namespace ENGINE;

TypeInfo *ComponentCollidable::mType = new TypeInfo("ComponentCollidable", &ComponentCollidable::createInstance);

ComponentCollidable::ComponentCollidable(Object *object, ParamMap &params) :
    Component(object, params, mType)
{

}

ComponentCollidable::~ComponentCollidable()
{
}

void *ComponentCollidable::createInstance(Object *object, ParamMap &params)
{
    return new ComponentCollidable(object, params);
}

void ComponentCollidable::init()
{
}
    
void ComponentCollidable::update(float elapsedTime)
{
}

void ComponentCollidable::receiveMessage(Message *message)
{
}
