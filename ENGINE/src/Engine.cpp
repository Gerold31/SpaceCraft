#include "Engine.hpp"

#include "MessageEngine.hpp"
#include "System.hpp"
#include "Defines.hpp"
#include "SystemLog.hpp"

#include "boost/date_time.hpp" 

using namespace ENGINE;

Engine::Engine() : 
    MessageReceiver(MessageReceiver::RECEIVER_ENGINE)
{
    LOG_IN("engine");
    mQuit = true;
    LOG_OUT("engine");
}

Engine::~Engine()
{
    LOG_IN("engine");
    mSystems.clear();
    LOG_OUT("engine");
}

void Engine::addSystem(System *system)
{
    LOG_IN("engine");
    LOG("add System" + system->getName(), "log");
    mSystems.push_back(system);
    LOG_OUT("engine");
}

System *Engine::getSystem(std::string name)
{
    LOG_IN("engine");
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        if((*i)->getName() == name)
        {
            LOG_OUT("engine");
            return *i;
        }
    }
    LOG_OUT("engine");
    return nullptr;
}

void Engine::init()
{
    LOG_IN("engine");
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        (*i)->init();
    }
    mQuit = false;
    LOG_OUT("engine");
}

void Engine::run()
{
    LOG_IN("engine");
    boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::local_time();

    while(!mQuit)
    {
        boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = t1 - t0;
        t0 = t1;

        update(diff.total_nanoseconds() * 1e-9);
    }
    LOG_OUT("engine");
}

void Engine::receiveMessage(Message *msg)
{
    LOG_IN_FRAME;
    if(MessageQuit *m = dynamic_cast<MessageQuit *>(msg))
    {
        mQuit = true;
    }
    LOG_OUT_FRAME;
}

void Engine::update(float elapsedTime)
{
    LOG_IN_FRAME;
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        (*i)->update(elapsedTime);
    }
    LOG_OUT_FRAME;
}
