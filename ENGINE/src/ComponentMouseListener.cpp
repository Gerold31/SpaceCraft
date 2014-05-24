#include "ComponentMouseListener.hpp"

#include "TypeInfo.hpp"
#include "SystemInput.hpp"
#include "SystemLog.hpp"
#include "MessageInput.hpp"

using namespace ENGINE;

TypeInfo *ComponentMouseListener::mType = new TypeInfo("ComponentMouseListener", &ComponentMouseListener::createInstance);

ComponentMouseListener::ComponentMouseListener(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mEnabled = true;
    LOG_OUT("component");
}

ComponentMouseListener::~ComponentMouseListener()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentMouseListener::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentMouseListener(object, params);
}

bool ComponentMouseListener::init()
{
    LOG_IN("component");
    SystemInput::getSingleton()->addMouseListener(this);
    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentMouseListener::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentMouseListener::receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageMouseEnable::getID())
    {
        MessageMouseEnable *msg = (MessageMouseEnable *)message;
        enable(msg->mEnable);
    }
    LOG_OUT_MSG;
}
