#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "MessageReceiver.hpp"

namespace ENGINE
{

class Message
{
public:
    Message() {}
    ~Message() {}

    virtual void sendTo(MessageReceiver *receiver) {receiver->receiveMessage(this);}

protected:
};

};

#endif
