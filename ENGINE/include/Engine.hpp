#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include <vector>

namespace ENGINE
{
    
class System;

class Engine
{
public:
    Engine();
    ~Engine();

    /**
        \note Ownership of the system goes to the engine
     */
    void addSystem(System *system);

    void init();
    void run();

private:
    void update(float elapsedTime);

    std::vector<System *> mSystems;

};

};

#endif
