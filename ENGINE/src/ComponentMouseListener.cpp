#include "ComponentMouseListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"

using namespace ENGINE;

TypeInfo *ComponentMouseListener::mType = new TypeInfo("ComponentMouseListener", &ComponentMouseListener::createInstance);

ComponentMouseListener::ComponentMouseListener(Object *object, std::map<std::string, std::string> params) :
    Component(object, params, mType)
{

}

ComponentMouseListener::~ComponentMouseListener()
{
}

void *ComponentMouseListener::createInstance(Object *object, std::map<std::string, std::string> params)
{
    return new ComponentMouseListener(object, params);
}

void ComponentMouseListener::init()
{
    SystemInput::getSingleton()->addMouseListener(this);
}
    
void ComponentMouseListener::update(float elapsedTime)
{
}

void ComponentMouseListener::receiveMessage(Message *message)
{
}
