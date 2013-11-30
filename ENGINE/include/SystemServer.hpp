#ifndef _SYSTEMSERVER_HPP_
#define _SYSTEMSERVER_HPP_

#include "System.hpp"

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/ServerSocket.h"

namespace ENGINE
{

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

private:
    SystemServer();
    SystemServer(const SystemServer &) : System("SystemServer"){}
    ~SystemServer();

    Poco::Net::TCPServer *mServer;
    Poco::Net::ServerSocket *mSocket;
};

};

#endif
