#include "SystemClient.hpp"

#include "Message.hpp"
#include "MessageInput.hpp"
#include "NetworkMessage.hpp"
#include "SystemConfiguration.hpp"

#include <sstream>
#include <cstring>

using namespace ENGINE;

SystemClient::SystemClient() :
    System("SystemClient")
{
}

SystemClient::~SystemClient()
{
}

void SystemClient::init()
{
    unsigned short port = atoi(SystemConfiguration::getSingleton()->getConfiguration("Port").c_str()); 
    mSocket = new Poco::Net::StreamSocket();
    mSocket->connect(Poco::Net::SocketAddress(SystemConfiguration::getSingleton()->getConfiguration("Host"), port));
}

void SystemClient::update(float elapsedTime)
{
    if(mSocket->available())
    {
    }
}

void SystemClient::receiveMessage(Message *msg)
{
    if(msg->getID() == MessageKeyPressed::getID())
    {
        MessageKeyPressed *m = (MessageKeyPressed *)msg;
        std::stringstream s;
        s << std::hex << m->mEvent.text;
        char buf[512];
        s >> buf;
        mSocket->sendBytes(buf, strlen(buf));
    }
}


void SystemClient::write(NetworkMessage *msg)
{
}
