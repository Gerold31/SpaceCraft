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
    MessageInterrupt(WORD msg) : Message(getID(), true, true, false, false, false), mMsg(msg) {}
    static void *CreateMessage(std::istream &stream) { WORD msg; stream >> msg; return new MessageInterrupt(msg); }
    static int getID() {if(mID == -1) mID = calcID("MessageInterrupt"); return mID;}
    WORD mMsg;
private:
    void _serialize(std::ostream &stream) { stream << mID << std::endl; }
    static int mID;
};

};

#endif
