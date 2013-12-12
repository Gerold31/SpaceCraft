#include "ComponentServerConnection.hpp"

#include "TypeInfo.hpp"
#include "SystemObjectFactory.hpp"
#include "Message.hpp"
#include "MessageEngine.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "SystemServer.hpp"

#include <iostream>
#include <string>
#include <exception>
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
    try{
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

            if(receiverName == "")
                break;

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
    }
    catch(Poco::Exception &e)
    {
        std::cout << "poco exception: " << e.name() << ": " << e.message() << std::endl;
    }
    catch(std::exception &e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
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
