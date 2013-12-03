#include "SystemClient.hpp"

#include "Message.hpp"
#include "SystemConfiguration.hpp"
#include "Object.hpp"

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"

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
}


void SystemClient::send(Message *msg, MessageReceiver *receiver)
{
    std::cout << "message to ";
    Poco::Net::SocketStream stream(*mSocket);

    switch(receiver->getReceiverType())
    {
    case MessageReceiver::RECEIVER_ENGINE:
        std::cout << "ENGINE" << std::endl;
        stream << MessageReceiver::RECEIVER_ENGINE << " ENGINE ";
        break;
    case MessageReceiver::RECEIVER_SYSTEM:
        std::cout << "System: " << ((System *)receiver)->getName() << std::endl;
        stream << MessageReceiver::RECEIVER_SYSTEM << " " << ((System *)receiver)->getName() << " ";
        break;
    case MessageReceiver::RECEIVER_OBJECT:
        std::cout << "Object: " << ((Object *)receiver)->getName() << std::endl;
        stream << MessageReceiver::RECEIVER_OBJECT << " " << ((Object *)receiver)->getName() << " ";
        break;
    default:
        // @todo no messages to components?
        break;
    }

    msg->serialize(stream);
}
