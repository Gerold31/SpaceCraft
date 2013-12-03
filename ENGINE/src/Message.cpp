#include "Message.hpp"

#include "SystemConfiguration.hpp"
#include "SystemClient.hpp"
#include "SystemServer.hpp"

#include <stdio.h>
#include <assert.h>

using namespace ENGINE;

std::map<int, CreateMessage> Message::mMessages;

void Message::sendTo(MessageReceiver *receiver)
{
    receiver->receiveMessage(this);
    if(mSendToServer && !SystemConfiguration::getSingleton()->isServer())
    {
        assert(SystemConfiguration::getSingleton()->isClient());
        SystemClient::getSingleton()->send(this, receiver);
    }
    if(mSendToClient && !SystemConfiguration::getSingleton()->isClient())
    {
        assert(SystemConfiguration::getSingleton()->isServer());
        // @todo
        //SystemServer::getSingleton()->send(this, receiver);
    }
}


int Message::calcID(std::string name)
{
    int id = 0;
    for(int i=0; i<name.size(); i++)
    {
        id += name[i] << (i % 38);
    }
    printf("Message %#.8x: \"%s\"\n", id, name.c_str());
    return id;
}

Message *Message::deserialize(std::istream &stream)
{
    int id;
    stream >> id;
    std::cout << "deserialize: " << id << std::endl;

    return (Message *)mMessages.at(id)(stream);
}