#ifndef _SYSTEMPHYSICS_HPP_
#define _SYSTEMPHYSICS_HPP_

#include "System.hpp"

namespace ENGINE
{

class SystemPhysics : public System, public Singleton<SystemPhysics>
{
friend class Singleton<SystemPhysics>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

private:
    SystemPhysics();
    SystemPhysics(const SystemPhysics &) {}
    ~SystemPhysics();

};

};

#endif
