#include "ComponentMemory.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace SpaceCraft;

TypeInfo *ComponentMemory::mType = new TypeInfo("ComponentMemory", &ComponentMemory::createInstance);

ComponentMemory::ComponentMemory(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    std::fstream file(boost::any_cast<std::string>(mParams["FileName"]), std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
    int memSize = sizeof(mMem)/sizeof(mMem[0]);
    if(file)
    {
        file.seekg (0, file.end);
        int length = file.tellg();
        file.seekg (0, file.beg);

        char *buf = new char [length];

        file.read(buf, length);

        int i;
        for(i=0; i<length/2 && i<memSize; i++)
        {
            mMem[i] = buf[i*2] & 0xFF;
            mMem[i] <<= 8;
            mMem[i] |= buf[i*2+1] & 0xFF;
        }

        if(i < memSize)
        {
            LOG("File to small", "warning");
            for(; i<memSize; i++)
            {
                mMem[i] = 0;
                file << '\0' << '\0';
            }
        }

        else if(length/2>memSize)
            LOG("File to big", "warning");

        delete buf;
        file.close();
    }else
    {
        for(int i=0; i<memSize; i++)
        {
            mMem[i] = 0;
        }
        LOG("Failed to open file", "error");
    }
    LOG_OUT("component");
}

ComponentMemory::~ComponentMemory()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentMemory::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentMemory(object, params);
}

bool ComponentMemory::init()
{
    LOG_IN("component");
    mReady = true;
    LOG_OUT("component");
    return true;
}

void ComponentMemory::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentMemory::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    LOG_OUT_MSG;
}
