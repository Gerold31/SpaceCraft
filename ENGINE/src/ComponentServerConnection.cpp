#include "ComponentServerConnection.hpp"

#include "TypeInfo.hpp"
#include "SystemObjectFactory.hpp"
#include "Message.hpp"
#include "MessageEngine.hpp"
#include "Engine.hpp"
#include "Object.hpp"

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

    while(1)
    {
        //if(sock.available())
        {
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
    }
    
    std::cout << "connection closed" << std::endl;
    
    
    // @todo destroy this object
    while(1);
}