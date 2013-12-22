#include "ComponentHardware.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "MessageCPU.hpp"

using namespace SpaceCraft;

ComponentHardware::ComponentHardware(Object *object, ParamMap &params, unsigned int id, unsigned int mf, WORD ver) :
    Component(object, params, mType),
    mID(id), mVER(ver), mMF(mf)
{
    LOG_IN("component");
    mCPU = nullptr;
    LOG_OUT("component");
}

ComponentHardware::~ComponentHardware()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void ComponentHardware::init()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void ComponentHardware::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentHardware::receiveMessage(Message *message)
{
    LOG_IN_FRAME;
    if(message->getID() == MessageInterrupt::getID())
    {
        interrupt();
    }
    LOG_OUT_FRAME;
}
