#include "ComponentServerConnection.hpp"

#include "TypeInfo.hpp"
#include "SystemObjectFactory.hpp"
#include "Message.hpp"
#include "MessageEngine.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "SystemServer.hpp"

#include <iostream>
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"

using namespace ENGINE;

TypeInfo *ComponentServerConnection::mType = new TypeInfo("ComponentServerConnection", &ComponentServerConnection::createInstance);

ComponentServerConnection::ComponentServerConnection(Object *object, ParamMap &params) :
    Component(object, params, mType),
    Poco::Net::TCPServerConnection(boost::any_cast<const Poco::Net::StreamSocket &>(params["Socket"]))
{
}

ComponentServerConnection::~ComponentServerConnection()
{
}

void *ComponentServerConnection::createInstance(Object *object, ParamMap &params)
{
    return new ComponentServerConnection(object, params);
}

void ComponentServerConnection::init()
{
}
    
void ComponentServerConnection::update(float elapsedTime)
{
}

void ComponentServerConnection::receiveMessage(Message *message)
{
}

void ComponentServerConnection::run()
{
    std::cout << "new incomming connection" << std::endl;
    Poco::Net::StreamSocket sock = socket();
    Poco::Net::SocketStream stream(sock);

    SystemServer::getSingleton()->addConnection(this);

    while(1)
    {
        // @todo remove code duplication at SystemClient::update
        int receiverType;
        std::string receiverName;
        stream >> receiverType;
        stream >> receiverName;

        std::cout << "Message from " << receiverType << ": "<< receiverName << std::endl;

        Message *message = Message::deserialize(stream);

        if(message->getID() == MessageQuit::getID())
        {
            break;
        }

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
    
    std::cout << "connection closed" << std::endl;
    
    SystemServer::getSingleton()->removeConnection(this);
    
    // @todo destroy this object
    while(1);
}

void ComponentServerConnection::send(Message *msg, MessageReceiver *receiver)
{
    // @todo remove code duplication at SystemClient::send
    std::cout << "message to ";
    Poco::Net::SocketStream stream(socket());

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