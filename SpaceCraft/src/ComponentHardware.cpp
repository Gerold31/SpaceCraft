#include "ComponentHardware.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "MessageCPU.hpp"

using namespace SpaceCraft;

ComponentHardware::ComponentHardware(Object *object, ParamMap &params, TypeInfo *type, unsigned int id, unsigned int mf, WORD ver) :
    Component(object, params, type),
    mID(id), mVER(ver), mMF(mf)
{
    LOG_IN("hardware");
    mCPU = nullptr;
    LOG_OUT("hardware");
}

ComponentHardware::~ComponentHardware()
{
    LOG_IN("hardware");
    LOG_OUT("hardware");
}

bool ComponentHardware::init()
{
    LOG_IN("hardware");
    mReady = true;
    LOG_OUT("hardware");
    return true;
}

void ComponentHardware::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentHardware::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageInterrupt::getID())
    {
        interrupt();
    }
    LOG_OUT_MSG;
}
