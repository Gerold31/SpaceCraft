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
    System(std::string name) : MessageReceiver(MessageReceiver::RECEIVER_SYSTEM), mName(name) {printf("create System %s\n", name.c_str());}
    System(const System&) : MessageReceiver(MessageReceiver::RECEIVER_SYSTEM) {}
    virtual ~System() {}

    std::string mName;
};

};

#endif
