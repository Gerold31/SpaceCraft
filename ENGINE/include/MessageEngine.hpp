#ifndef _MESSAGEQUIT_HPP_
#define _MESSAGEQUIT_HPP_

#include "Message.hpp"

namespace ENGINE
{

class MessageQuit : public Message
{
public:
    MessageQuit() : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; LOG_OUT_MSG; return new MessageQuit();}
    static int getID() {if(mID == -1) mID = calcID("MessageQuit"); return mID;}
private:
    static int mID;
};

};

#endif
