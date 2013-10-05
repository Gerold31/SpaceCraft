#include "Engine.hpp"

#include "System.hpp"

#include "boost/date_time.hpp" 

using namespace ENGINE;

Engine::Engine()
{
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

void Engine::init()
{
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        (*i)->init();
    }
}

void Engine::run()
{
    boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::local_time();

    while(1)
    {
        boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff = t1 - t0;
        t0 = t1;

        update(diff.total_milliseconds() / 1000.0f);
    }
}

void Engine::update(float elapsedTime)
{
    for(std::vector<System *>::iterator i = mSystems.begin(); i!=mSystems.end(); ++i)
    {
        (*i)->update(elapsedTime);
    }
}
