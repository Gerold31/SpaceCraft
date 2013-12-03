#include "SystemServer.hpp"

#include "SystemGameState.hpp"
#include "SystemConfiguration.hpp"
#include "ComponentServerConnection.hpp"
#include "Object.hpp"

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
