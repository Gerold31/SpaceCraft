#include "SystemClient.hpp"

#include "Message.hpp"
#include "SystemConfiguration.hpp"
#include "Object.hpp"
#include "Engine.hpp"
#include "MessageEngine.hpp"

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
    mSocket->setBlocking(false);
    mStream = new Poco::Net::SocketStream(*mSocket);
}

void SystemClient::update(float elapsedTime)
{
    try
    {
        // @todo remove code duplication at ComponentServerConnection::run
        int receiverType;
        std::string receiverName;
        (*mStream) >> receiverType;
        (*mStream) >> receiverName;

        std::cout << "Message from " << receiverType << ": "<< receiverName << std::endl;

        if(receiverName == "")
            return;

        Message *message = Message::deserialize(*mStream);

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
    catch(Poco::TimeoutException &e)
    {
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
        stream << MessageReceiver::RECEIVER_ENGINE  << std::endl << "ENGINE" << std::endl;
        break;
    case MessageReceiver::RECEIVER_SYSTEM:
        std::cout << "System: " << ((System *)receiver)->getName() << std::endl;
        stream << MessageReceiver::RECEIVER_SYSTEM << std::endl << ((System *)receiver)->getName() << std::endl;
        break;
    case MessageReceiver::RECEIVER_OBJECT:
        std::cout << "Object: " << ((Object *)receiver)->getName() << std::endl;
        stream << MessageReceiver::RECEIVER_OBJECT << std::endl << ((Object *)receiver)->getName() << std::endl;
        break;
    default:
        // @todo no messages to components?
        break;
    }
    msg->serialize(stream);

    stream.flush();
}
