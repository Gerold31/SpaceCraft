#include "SystemClient.hpp"

#include "Message.hpp"
#include "SystemConfiguration.hpp"
#include "Object.hpp"
#include "Engine.hpp"
#include "MessageEngine.hpp"
#include "SystemObjectFactory.hpp"

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

    mThread.start(*this);
}

void SystemClient::update(float elapsedTime)
{
}

void SystemClient::receiveMessage(Message *msg)
{
}

void SystemClient::run()
{
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

        std::cout << "connection closed" << std::endl;
    }
    catch(Poco::Exception &e)
    {
        std::cout << "poco exception: " << e.name() << ": " << e.message() << std::endl;
    }
    catch(std::exception &e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

void SystemClient::send(Message *msg, MessageReceiver *receiver)
{
    // @todo remove code duplication at ComponentServerConnection::send
    try{
        Poco::Net::SocketStream stream(*mSocket);

        std::cout << "send Message to ";

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

        std::cout << "serialize msg..." << std::endl;
        msg->serialize(stream);

        std::cout << "flush" << std::endl;

        stream.flush();
    }
    catch(Poco::Exception &e)
    {
        std::cout << "poco exception: " << e.name() << ": " << e.message() << std::endl;
    }
    catch(std::exception &e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "unknown exception" << std::endl;
    }
}
