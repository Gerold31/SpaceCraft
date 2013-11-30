#ifndef _SYSTEMCLIENT_HPP_
#define _SYSTEMCLIENT_HPP_

#include "System.hpp"

#include "Poco/Net/StreamSocket.h"

namespace ENGINE
{

class NetworkMessage;

class SystemClient : public System, public Singleton<SystemClient>
{
friend class Singleton<SystemClient>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    void write(NetworkMessage *msg);

private:
    SystemClient();
    SystemClient(const SystemClient &):System("SystemClient") {}
    ~SystemClient();

    Poco::Net::StreamSocket *mSocket;

};

};

#endif
