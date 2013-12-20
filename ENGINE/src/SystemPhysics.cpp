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
}

void SystemPhysics::receiveMessage(Message *msg)
{
}
