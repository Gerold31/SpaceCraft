#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include <string>
#include <iostream>
#include <map>

#include "MessageReceiver.hpp"

namespace ENGINE
{
    
typedef void* (*CreateMessage)(std::istream &stream);

class Message
{
public:
    Message(int id, bool sendToServer, bool sendToClient) : mID(id), mSendToServer(sendToServer), mSendToClient(sendToClient) {}
    virtual ~Message() {};

    void sendTo(MessageReceiver *receiver);

    void serialize(std::ostream &stream) {stream << mID << " "; std::cout << "serialize: " << mID << std::endl; _serialize(stream);}
    static Message *deserialize(std::istream &stream); 

    int getID() {return mID;}
    
    static void registerMessge(int id, CreateMessage createMessage)
    {
        mMessages[id] = createMessage;
    }

protected:
    static int calcID(std::string name);
    virtual void _serialize(std::ostream &stream) {};

    int mID;
    bool mSendToServer, mSendToClient;
    
    static std::map<int, CreateMessage> mMessages;
};

};

#endif
