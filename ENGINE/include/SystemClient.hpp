#ifndef _SYSTEMCLIENT_HPP_
#define _SYSTEMCLIENT_HPP_

#include "System.hpp"

#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"

namespace ENGINE
{

class SystemClient : public System, public Singleton<SystemClient>, public Poco::Runnable
{
friend class Singleton<SystemClient>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    void send(Message *msg, MessageReceiver *receiver);

    void run();

private:
    SystemClient();
    SystemClient(const SystemClient &):System("SystemClient") {}
    ~SystemClient();

    Poco::Net::StreamSocket *mSocket;
    Poco::Net::SocketStream *mStream;
    Poco::Thread mThread;

};

};

#endif
