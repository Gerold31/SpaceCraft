#ifndef _MESSAGERECEIVER_HPP_
#define _MESSAGERECEIVER_HPP_

namespace ENGINE
{

class Message;

class MessageReceiver
{
public:
    enum RECEIVER_TYPE
    {
        RECEIVER_ENGINE,
        RECEIVER_SYSTEM,
        RECEIVER_OBJECT,
        RECEIVER_COMPONENT
    };

    MessageReceiver(RECEIVER_TYPE type) : mReceiverType(type) {}
    virtual void receiveMessage(Message *msg) = 0;
    RECEIVER_TYPE getReceiverType() {return mReceiverType;}

private:
    RECEIVER_TYPE mReceiverType;
};

};

#endif
