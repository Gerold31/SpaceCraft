#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

#include "MessageReceiver.hpp"
#include "Singleton.hpp"

#include <stdio.h>
#include <string>

namespace ENGINE
{

class System : public MessageReceiver
{
public:
    virtual void init() = 0;
    virtual void update(float elapsedTime) = 0;
    std::string getName() {return mName;}

protected:
    System(std::string name) :mName(name) {printf("create System\n");}
    System(const System&) {}
    virtual ~System() {}

    std::string mName;
};

};

#endif
