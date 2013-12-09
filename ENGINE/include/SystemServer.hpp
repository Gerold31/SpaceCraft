#ifndef _SYSTEMSERVER_HPP_
#define _SYSTEMSERVER_HPP_

#include "System.hpp"

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/ServerSocket.h"

namespace ENGINE
{

class ComponentServerConnection;

class ServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
    ServerConnectionFactory() {};

    Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket);
};

class SystemServer : public System, public Singleton<SystemServer>
{
friend class Singleton<SystemServer>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);
    
    void addConnection(ComponentServerConnection *connection);
    void removeConnection(ComponentServerConnection *connection);
    
    void sendTo(Message *msg, MessageReceiver *receiver, ComponentServerConnection *to);
    void sendToAllBut(Message *msg, MessageReceiver *receiver, ComponentServerConnection *notTo);
    void sendToAll(Message *msg, MessageReceiver *receiver);

private:
    SystemServer();
    SystemServer(const SystemServer &) : System("SystemServer"){}
    ~SystemServer();

    Poco::Net::TCPServer *mServer;
    Poco::Net::ServerSocket *mSocket;

    std::vector<ComponentServerConnection *> mConnections;
};

};

#endif
