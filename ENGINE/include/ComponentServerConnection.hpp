#ifndef _COMPONENTSERVERCONNECTION_HPP_
#define _COMPONENTSERVERCONNECTION_HPP_

#include "Component.hpp"

#include "Poco/Net/TCPServerConnection.h"

namespace ENGINE
{

class ComponentServerConnection : public Component, public Poco::Net::TCPServerConnection
{
public:
    ComponentServerConnection(Object *object, ParamMap &params);
    ~ComponentServerConnection();

    static void *createInstance(Object *object, ParamMap &params);
    
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    void run();

    void send(Message *msg, MessageReceiver *receiver);

    static TypeInfo *getType() {return mType;}

private:
    static TypeInfo *mType;

};

};

#endif