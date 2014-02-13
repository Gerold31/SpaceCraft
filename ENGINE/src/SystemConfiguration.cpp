#include "SystemConfiguration.hpp"

#include "SystemLog.hpp"

#include <fstream>

using namespace ENGINE;

SystemConfiguration::SystemConfiguration() :
    System("SystemConfiguration")
{
    LOG_IN("system");
    LOG_OUT("system");
}

SystemConfiguration::~SystemConfiguration()
{
    LOG_IN("system");
    LOG_OUT("system");
}

void SystemConfiguration::init()
{
    LOG_IN("system");
    LOG_OUT("system");
}
    
void SystemConfiguration::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void SystemConfiguration::receiveMessage(Message *msg)
{
    LOG_IN_MSG;
    LOG_OUT_MSG;
}

void SystemConfiguration::loadFromFile(std::string filename)
{
    LOG_IN("system");
    std::fstream file;

    file.open(filename);

    if(file.eof() || !file.is_open())
    {
        file.close();
        LOG_OUT("system");
        return;
    }

    while(!file.eof())
    {
        std::string param,value;

        file >> param;
        file >> value;

        mConfiguration[param] = value;
        LOG("Add Param " + param + " = " + value, "log");
        if(param == "NetworkType")
        {
            if(value == "Client")
            {
                mIsServer = false;
                mIsClient = true;
            }else if(value == "ListenServer")
            {
                mIsServer = true;
                mIsClient = true;
            }else if(value == "DedicatedServer")
            {
                mIsServer = true;
                mIsClient = false;
            }
        }
    }

    file.close();
    LOG_OUT("system");
}

std::string SystemConfiguration::getConfiguration(std::string name)
{
    LOG_IN("system");
    LOG_OUT("system");
    return mConfiguration.at(name);
}
