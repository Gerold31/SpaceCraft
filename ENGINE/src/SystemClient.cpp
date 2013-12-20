#include "SystemClient.hpp"

#include "Message.hpp"
#include "SystemConfiguration.hpp"
#include "Object.hpp"
#include "Engine.hpp"
#include "MessageEngine.hpp"
#include "SystemObjectFactory.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

SystemClient::SystemClient() :
    System("SystemClient")
{
    LOG_IN("system");
    LOG_OUT("system");
}

SystemClient::~SystemClient()
{
    LOG_IN("system");
    LOG_OUT("system");
}

void SystemClient::init()
{
    LOG_IN("system");
    unsigned short port = atoi(SystemConfiguration::getSingleton()->getConfiguration("Port").c_str()); 
    mSocket = new Poco::Net::StreamSocket();
    mSocket->connect(Poco::Net::SocketAddress(SystemConfiguration::getSingleton()->getConfiguration("Host"), port));

    mThread.start(*this);
    LOG_OUT("system");
}

void SystemClient::update(float elapsedTime)
{
}

void SystemClient::receiveMessage(Message *msg)
{
}

void SystemClient::run()
{
    LOG_IN("system");
    try{
        Poco::Net::SocketStream stream(*mSocket);
        
        while(1)
        {
            // @todo remove code duplication at ComponentServerConnection::run

            int receiverType;
            std::string receiverName;
            stream >> receiverType;
            stream >> receiverName;

            if(receiverName == "")
                break;

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
    }
    catch(Poco::Exception &e)
    {
        LOG(e.what(), "error");
    }
    catch(std::exception &e)
    {
        LOG(e.what(), "error");
    }
    LOG_OUT("system");
}

void SystemClient::send(Message *msg, MessageReceiver *receiver)
{
    LOG_IN("system");
    // @todo remove code duplication at ComponentServerConnection::send
    try{
        Poco::Net::SocketStream stream(*mSocket);

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
    LOG_OUT("system");
}
