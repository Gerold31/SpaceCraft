#include "SystemConfiguration.hpp"

#include <fstream>

using namespace ENGINE;

SystemConfiguration::SystemConfiguration() :
    System("SystemConfiguration")
{
}

SystemConfiguration::~SystemConfiguration()
{
}

void SystemConfiguration::init()
{
}
    
void SystemConfiguration::update(float elapsedTime)
{
}

void SystemConfiguration::receiveMessage(Message *msg)
{
}

void SystemConfiguration::loadFromFile(std::string filename)
{
    std::fstream file;

    file.open(filename);

    if(file.eof() || !file.is_open())
    {
        file.close();
        return;
    }

    while(!file.eof())
    {
        std::string param,value;

        file >> param;
        file >> value;

        mConfiguration[param] = value;
        printf("Add Param %s = %s\n", param.c_str(), value.c_str());
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
}

std::string SystemConfiguration::getConfiguration(std::string name)
{
    return mConfiguration.at(name);
}
