#ifndef _MESSAGECPU_HPP_
#define _MESSAGECPU_HPP_

#include "SpaceCraftDefines.hpp"
#include "Message.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class MessageInterrupt : public Message
{
public:
    MessageInterrupt(WORD msg) : Message(getID(), true, true, false, false, false), mMsg(msg) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; WORD msg; stream >> msg; return new MessageInterrupt(msg); LOG_OUT_MSG;}
    static int getID() {if(mID == -1) mID = calcID("MessageInterrupt"); return mID;}
    WORD mMsg;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; stream << mID << std::endl; LOG_OUT_MSG;}
    static int mID;
};

};

#endif
