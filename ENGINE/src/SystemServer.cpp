#include "SystemServer.hpp"

#include "SystemGameState.hpp"
#include "SystemConfiguration.hpp"
#include "ComponentServerConnection.hpp"
#include "Object.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

Poco::Net::TCPServerConnection* ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket)
{
    LOG_IN("system");
    ParamMap params;
    params["Socket"] = socket;

    ComponentServerConnection *connection = SystemGameState::getSingleton()->newPlayer(params);

    LOG_OUT("system");
    return connection;
}

SystemServer::SystemServer() :
    System("SystemServer")
{
    LOG_IN("system");
    LOG_OUT("system");
}

SystemServer::~SystemServer()
{
    LOG_IN("system");
    mServer->stop();
    LOG_OUT("system");
}

void SystemServer::init()
{
    LOG_IN("system");
    unsigned short port = atoi(SystemConfiguration::getSingleton()->getConfiguration("Port").c_str()); 
    mSocket = new Poco::Net::ServerSocket(port);
    mServer = new Poco::Net::TCPServer(new ServerConnectionFactory(), *mSocket);
    mServer->start();
    LOG_OUT("system");
}

void SystemServer::update(float elapsedTime)
{
}

void SystemServer::receiveMessage(Message *msg)
{
}

void SystemServer::addConnection(ComponentServerConnection *connection)
{
    LOG_IN("system");
    mConnections.push_back(connection);
    LOG_OUT("system");
}

void SystemServer::removeConnection(ComponentServerConnection *connection)
{
    LOG_IN("system");
    for(auto i=mConnections.begin(); i!=mConnections.end(); ++i)
    {
        if(*i == connection)
        {
            mConnections.erase(i);
            LOG_OUT("system");
            return;
        }
    }
    LOG_OUT("system");
}

void SystemServer::sendTo(Message *msg, MessageReceiver *receiver, ComponentServerConnection *to)
{
    LOG_IN("system");
    to->send(msg, receiver);
    LOG_OUT("system");
}

void SystemServer::sendToAllBut(Message *msg, MessageReceiver *receiver, ComponentServerConnection *notTo)
{
    LOG_IN("system");
    for(auto i=mConnections.begin(); i!=mConnections.end(); ++i)
    {
        if(*i != notTo)
        {
            (*i)->send(msg, receiver);
        }
    }
    LOG_OUT("system");
}

void SystemServer::sendToAll(Message *msg, MessageReceiver *receiver)
{
    LOG_IN("system");
    for(auto i=mConnections.begin(); i!=mConnections.end(); ++i)
    {
        (*i)->send(msg, receiver);
    }
    LOG_OUT("system");
}
