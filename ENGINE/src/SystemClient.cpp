#include "SystemClient.hpp"

#include "Message.hpp"
#include "MessageInput.hpp"
#include "NetworkMessage.hpp"

#include <sstream>

#define HOST ("192.168.0.13")
#define PORT (55555)

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
    mSocket = new Poco::Net::StreamSocket();
    mSocket->connect(Poco::Net::SocketAddress(HOST, PORT));
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