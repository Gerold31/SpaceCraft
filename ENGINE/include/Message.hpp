#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <string>
#include <iostream>
#include <map>

#include "Defines.hpp"
#include "SystemLog.hpp"
#include "MessageReceiver.hpp"

namespace ENGINE
{
    
typedef void* (*CreateMessage)(std::istream &stream);

class Message
{
public:
    Message(int id, bool localToServer, bool localToClient, bool sendToServer, bool sendToClient, bool distribute) :
        mID(id), mLocalToServer(localToServer), mLocalToClient(localToClient), mSendToServer(sendToServer), mSendToClient(sendToClient), mDistribute(distribute) {}
    virtual ~Message() {};

    void sendTo(MessageReceiver *receiver);

    void serialize(std::ostream &stream);
    static Message *deserialize(std::istream &stream); 

    const int getID() {return mID;}
    const bool getDistribute() {return mDistribute;}
    
    static void registerMessge(int id, CreateMessage createMessage)
    {
        LOG_IN_MSG;
        mMessages[id] = createMessage;
        LOG_OUT_MSG;
    }

protected:
    static int calcID(std::string name);
    virtual void _serialize(std::ostream &stream) {};

    const int mID;
    const bool mLocalToServer, mLocalToClient;
    const bool mSendToServer, mSendToClient;
    const bool mDistribute;
    
    static std::map<int, CreateMessage> mMessages;
};

};

#endif
