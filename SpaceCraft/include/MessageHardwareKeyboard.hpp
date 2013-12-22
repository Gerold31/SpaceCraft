#ifndef _MESSAGEHARDWAREKEYBOARD_HPP_
#define _MESSAGEHARDWAREKEYBOARD_HPP_

#include "SpaceCraftDefines.hpp"
#include "Message.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class MessageHardwareKeyPressed : public Message
{
public:
    MessageHardwareKeyPressed(WORD key) : Message(getID(), true, false, true, false, false), mKey(key) {}
    static void *CreateMessage(std::istream &stream) { WORD key; stream >> key; return new MessageHardwareKeyPressed(key); }
    static int getID() {if(mID == -1) mID = calcID("MessageHardwareKeyPressed"); return mID;}
    WORD mKey;
private:
    void _serialize(std::ostream &stream) { stream << mKey << std::endl; };
    static int mID;
};

class MessageHardwareKeyReleased : public Message
{
public:
    MessageHardwareKeyReleased(WORD key) : Message(getID(), true, false, true, false, false), mKey(key) {}
    static void *CreateMessage(std::istream &stream) { WORD key; stream >> key; return new MessageHardwareKeyReleased(key); }
    static int getID() {if(mID == -1) mID = calcID("MessageHardwareKeyPressed"); return mID;}
    WORD mKey;
private:
    void _serialize(std::ostream &stream) { stream << mKey << std::endl; };
    static int mID;
};

};

#endif
