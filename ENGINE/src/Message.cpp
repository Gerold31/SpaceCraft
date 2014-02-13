#include "Message.hpp"

#include "SystemConfiguration.hpp"
#include "SystemClient.hpp"
#include "SystemServer.hpp"
#include "SystemLog.hpp"

#include <assert.h>

using namespace ENGINE;

std::map<int, CreateMessage> Message::mMessages;

void Message::sendTo(MessageReceiver *receiver)
{
    if((mLocalToServer && SystemConfiguration::getSingleton()->isServer()) || (mLocalToClient && SystemConfiguration::getSingleton()->isClient()))
    {
        receiver->receiveMessage(this);
    }
    if(mSendToServer && !SystemConfiguration::getSingleton()->isServer())
    {
        assert(SystemConfiguration::getSingleton()->isClient());
        SystemClient::getSingleton()->send(this, receiver);
    }
    if(mSendToClient && !SystemConfiguration::getSingleton()->isClient())
    {
        assert(SystemConfiguration::getSingleton()->isServer());
        SystemServer::getSingleton()->sendToAll(this, receiver);
    }
}

int Message::calcID(std::string name)
{
    LOG_IN("message");
    int id = 0;
    for(int i=0; i<name.size(); i++)
    {
        id += name[i] << (i % (32-7));
    }
    LOG_OUT("message");
    return id;
}

void Message::serialize(std::ostream &stream) 
{
    LOG_IN_MSG;
    stream << mID << std::endl;
    //LOG("serialize: " << std::hex << mID << std::endl;
    _serialize(stream);
    LOG_OUT_MSG;
}

Message *Message::deserialize(std::istream &stream)
{
    LOG_IN_MSG;
    int id;
    stream >> id;

    Message *m = (Message *)mMessages.at(id)(stream);

    LOG_OUT_MSG;
    return m;
}
