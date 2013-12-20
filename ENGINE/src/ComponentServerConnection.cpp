#include "ComponentServerConnection.hpp"

#include "TypeInfo.hpp"
#include "SystemObjectFactory.hpp"
#include "Message.hpp"
#include "MessageEngine.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "SystemServer.hpp"
#include "SystemLog.hpp"

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
    LOG_IN("component");
    LOG_OUT("component");
}

ComponentServerConnection::~ComponentServerConnection()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentServerConnection::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentServerConnection(object, params);
}

void ComponentServerConnection::init()
{
    LOG_IN("component");
    LOG_OUT("component");
}
    
void ComponentServerConnection::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentServerConnection::receiveMessage(Message *message)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentServerConnection::run()
{
    LOG_IN("component");
    try{
        LOG("new incomming connection", "log");
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
                r = SystemObjectFactory::getSingleton()->getObject(receiverName);
                break;
            default:
                // @todo no messages to components?
                r = nullptr;
            }

            if(r)
                r->receiveMessage(message);

            delete message;
        }

        LOG("connection closed", "log");

        SystemServer::getSingleton()->removeConnection(this);
    }
    catch(Poco::Exception &e)
    {
        LOG(e.what(), "error");
    }
    catch(std::exception &e)
    {
        LOG(e.what(), "error");
    }
    catch(...)
    {
        LOG("unknown exception", "error");
    }

    LOG_OUT("component");
    // @todo destroy this object
    while(1);
}

void ComponentServerConnection::send(Message *msg, MessageReceiver *receiver)
{
    LOG_IN("component");
    // @todo remove code duplication at SystemClient::send
    try{
        Poco::Net::SocketStream stream(socket());

        switch(receiver->getReceiverType())
        {
        case MessageReceiver::RECEIVER_ENGINE:
            LOG("Send to: ENGINE", "log");
            stream << MessageReceiver::RECEIVER_ENGINE  << std::endl << "ENGINE" << std::endl;
            break;
        case MessageReceiver::RECEIVER_SYSTEM:
            LOG("Send to: System", "log");
            stream << MessageReceiver::RECEIVER_SYSTEM << std::endl << ((System *)receiver)->getName() << std::endl;
            break;
        case MessageReceiver::RECEIVER_OBJECT:
            LOG("Send to: Object", "log");
            stream << MessageReceiver::RECEIVER_OBJECT << std::endl << ((Object *)receiver)->getName() << std::endl;
            break;
        default:
            // @todo no messages to components?
            break;
        }

        LOG("serialize msg...", "log");
        msg->serialize(stream);

        LOG("flush", "log");

        stream.flush();
    }
    catch(Poco::Exception &e)
    {
        LOG(e.what(), "error");
    }
    catch(std::exception &e)
    {
        LOG(e.what(), "error");
    }
    catch(...)
    {
        LOG("unknown exception", "error");
    }
    LOG_OUT("component");
}
