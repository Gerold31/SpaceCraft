#include "ComponentServerConnection.hpp"

#include "TypeInfo.hpp"
#include "SystemObjectFactory.hpp"

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

    Poco::Net::SocketStream stream(socket());
    Poco::Net::StreamSocket sock = socket();

    bool quit = false;

    while(!quit)
    {
        if(sock.available())
        {
            char buf[512];
            sock.receiveBytes(buf, 512);
            std::cout << "received:\n\t" << buf << std::endl;
            if(strcmp(buf, "quit") == 0)
                quit = true;
        }
    }
    

    std::cout << "connection closed" << std::endl;

    // destroy this object
    SystemObjectFactory::getSingleton();
}