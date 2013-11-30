#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <string>

#include "MessageReceiver.hpp"

namespace ENGINE
{

class NetworkMessage;

class Message
{
public:
    enum RECEIVER_TYPE
    {
        RECEIVER_ENGINE,
        RECEIVER_SYSTEM,
        RECEIVER_OBJECT
    };

    Message(int id) : mID(id) {}
    virtual ~Message() {};

    void sendTo(MessageReceiver *receiver) {receiver->receiveMessage(this);}

    virtual NetworkMessage *toNetworkMessage();

    int getID() {return mID;}

protected:
    static int calcID(std::string name);

    int mID;
    RECEIVER_TYPE mReceiverType;
    std::string mReceiverName;
};

};

#endif
