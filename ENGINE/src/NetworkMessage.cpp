#include "NetworkMessage.hpp"

#include "Message.hpp"
#include "MessageReceiver.hpp"
#include "Engine.hpp"
#include "SystemObjectFactory.hpp"
#include "Object.hpp"

#include <sstream>

using namespace ENGINE;

std::map<int, CreateMessage> NetworkMessage::mMessages;

void NetworkMessage::deserialize()
{
    /*
    std::stringstream stream;
    stream.str(mData);
    int receiverType;
    std::string receiverName;
    stream >> receiverType;
    stream >> receiverName;

    int id;
    stream >> id;
    Message *message = (Message *)mMessages[id](stream);

    MessageReceiver *r = NULL;
    switch(receiverType)
    {
    case Message::RECEIVER_ENGINE:
        r = Engine::getSingleton();
        break;
    case Message::RECEIVER_SYSTEM:
        r = Engine::getSingleton()->getSystem(receiverName);
        break;
    case Message::RECEIVER_OBJECT:
        r = SystemObjectFactory::getSingleton()->getObject(receiverName);
        break;
    default:
        r = NULL;
    }
    if(r)
        r->receiveMessage(message);

    delete message;
    */
}