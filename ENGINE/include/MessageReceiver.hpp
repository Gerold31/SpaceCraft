#ifndef _MESSAGERECEIVER_HPP_
#define _MESSAGERECEIVER_HPP_

namespace ENGINE
{

class Message;

class MessageReceiver
{
public:
    virtual void receiveMessage(Message *msg) = 0;

private:
};

};

#endif
