#include "Engine.hpp"

#include "MessageEngine.hpp"
#include "System.hpp"

#include "boost/date_time.hpp" 

using namespace ENGINE;

Engine::Engine()
{
    mQuit = true;
}

Engine::~Engine()
{
    mSystems.clear();
}

void Engine::addSystem(System *system)
{
    printf("add System\n");
    mSystems.push_back(system);
}

System *Engine::getSystem(std::string name)
{
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        if((*i)->getName() == name)
            return *i;
    }
    return nullptr;
}

void Engine::init()
{
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        (*i)->init();
    }
    mQuit = false;
}

void Engine::run()
{
    boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::local_time();

    while(!mQuit)
    {
        boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = t1 - t0;
        t0 = t1;

        update(diff.total_milliseconds() / 1000.0f);
    }
}

void Engine::receiveMessage(Message *msg)
{
    if(MessageQuit *m = dynamic_cast<MessageQuit *>(msg))
    {
        mQuit = true;
    }
}

void Engine::update(float elapsedTime)
{
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        (*i)->update(elapsedTime);
    }
}
