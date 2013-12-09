#include "SystemClient.hpp"

#include "Message.hpp"
#include "SystemConfiguration.hpp"
#include "Object.hpp"
#include "Engine.hpp"

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
    Poco::Net::SocketStream stream(*mSocket);
    while(mSocket->available())
    {
        // @todo remove code duplication at ComponentServerConnection::run
        int receiverType;
        std::string receiverName;
        stream >> receiverType;
        stream >> receiverName;

        std::cout << "Message from " << receiverType << ": "<< receiverName << std::endl;

        Message *message = Message::deserialize(stream);

        MessageReceiver *r = nullptr;
        switch(receiverType)
        {
        case MessageReceiver::RECEIVER_ENGINE:
            r = Engine::getSingleton();
            break;
        case MessageReceiver::RECEIVER_SYSTEM:
            r = Engine::getSingleton()->getSystem(receiverName);
            break;
        case MessageReceiver::RECEIVER_OBJECT:
            //r = SystemObjectFactory::getSingleton()->getObject(receiverName);
            break;
        default:
            // @todo no messages to components?
            r = nullptr;
        }

        if(r)
            r->receiveMessage(message);

        delete message;
    }
}

void SystemClient::receiveMessage(Message *msg)
{
}


void SystemClient::send(Message *msg, MessageReceiver *receiver)
{
    // @todo remove code duplication at ComponentServerConnection::send
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
