#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include "MessageReceiver.hpp"
#include "Singleton.hpp"

#include <vector>
#include <string>

namespace ENGINE
{
    
class System;

class Engine : public Singleton<Engine>, public MessageReceiver
{
friend class Singleton<Engine>;
public:
    /**
        \note Ownership of the system goes to the engine
     */
    void addSystem(System *system);
    System *getSystem(std::string name);

    void init();
    void run();

    void receiveMessage(Message *msg);

private:
    Engine();
    ~Engine();

    void update(float elapsedTime);

    std::vector<System *> mSystems;

    bool mQuit;

};

};

#endif
