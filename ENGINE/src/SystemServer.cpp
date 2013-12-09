#include "SystemServer.hpp"

#include "SystemGameState.hpp"
#include "SystemConfiguration.hpp"
#include "ComponentServerConnection.hpp"
#include "Object.hpp"

#include <iostream>

using namespace ENGINE;

Poco::Net::TCPServerConnection* ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket)
{
    ParamMap params;
    params["Socket"] = socket;

    ComponentServerConnection *connection = SystemGameState::getSingleton()->newPlayer(params);

    return connection;
}

SystemServer::SystemServer() :
    System("SystemServer")
{
}

SystemServer::~SystemServer()
{
    mServer->stop();
}

void SystemServer::init()
{
    unsigned short port = atoi(SystemConfiguration::getSingleton()->getConfiguration("Port").c_str()); 
    mSocket = new Poco::Net::ServerSocket(port);
    mServer = new Poco::Net::TCPServer(new ServerConnectionFactory(), *mSocket);
    mServer->start();
}

void SystemServer::update(float elapsedTime)
{
}

void SystemServer::receiveMessage(Message *msg)
{
}

void SystemServer::addConnection(ComponentServerConnection *connection)
{
    mConnections.push_back(connection);
}

void SystemServer::removeConnection(ComponentServerConnection *connection)
{
    for(auto i=mConnections.begin(); i!=mConnections.end(); ++i)
    {
        if(*i == connection)
        {
            mConnections.erase(i);
            return;
        }
    }
}

void SystemServer::sendTo(Message *msg, MessageReceiver *receiver, ComponentServerConnection *to)
{
    to->send(msg, receiver);
}

void SystemServer::sendToAllBut(Message *msg, MessageReceiver *receiver, ComponentServerConnection *notTo)
{
    for(auto i=mConnections.begin(); i!=mConnections.end(); ++i)
    {
        if(*i != notTo)
        {
            (*i)->send(msg, receiver);
        }
    }
}

void SystemServer::sendToAll(Message *msg, MessageReceiver *receiver)
{
    for(auto i=mConnections.begin(); i!=mConnections.end(); ++i)
    {
        (*i)->send(msg, receiver);
    }
}