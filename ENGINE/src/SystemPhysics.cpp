#include "SystemPhysics.hpp"

#include "SystemLog.hpp"

using namespace ENGINE;

SystemPhysics::SystemPhysics() :
    System("SystemPhysics")
{
    LOG_IN("system");
    LOG_OUT("system");
}

SystemPhysics::~SystemPhysics()
{
    LOG_IN("system");
    LOG_OUT("system");
}

void SystemPhysics::init()
{
    LOG_IN("system");
    LOG_OUT("system");
}
    
void SystemPhysics::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void SystemPhysics::receiveMessage(Message *msg)
{
    LOG_IN_MSG;
    LOG_OUT_MSG;
}
