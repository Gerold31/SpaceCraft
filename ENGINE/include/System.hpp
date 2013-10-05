#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "MessageReceiver.hpp"
#include "Singleton.hpp"

#include <stdio.h>

namespace ENGINE
{

class System : public MessageReceiver
{
public:
    virtual void init() = 0;
    virtual void update(float elapsedTime) = 0;

protected:
    System() {printf("create System\n");}
    System(const System&) {}
    virtual ~System() {}

};

};

#endif
