#ifndef _MESSAGEQUIT_HPP_
#define _MESSAGEQUIT_HPP_

#include "Message.hpp"

#include <OIS/OIS.h>

namespace ENGINE
{

class MessageQuit : public Message
{
public:
    MessageQuit() : Message(getID(), true, true) {}
    static void *CreateMessage(std::istream &stream) { return new MessageQuit(); }
    static int getID() {if(mID == -1) mID = calcID("MessageQuit"); return mID;}
private:
    static int mID;
};

};

#endif
