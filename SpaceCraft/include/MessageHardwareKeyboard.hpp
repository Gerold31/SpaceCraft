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
    MessageHardwareKeyPressed(WORD key) : Message(getID(), true, false, true, false, false), mKey(key) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; WORD key; stream >> key; return new MessageHardwareKeyPressed(key); LOG_OUT_MSG;}
    static int getID() {if(mID == -1) mID = calcID("MessageHardwareKeyPressed"); return mID;}
    WORD mKey;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG;  stream << mKey << std::endl; LOG_OUT_MSG;}
    static int mID;
};

class MessageHardwareKeyReleased : public Message
{
public:
    MessageHardwareKeyReleased(WORD key) : Message(getID(), true, false, true, false, false), mKey(key) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG;  WORD key; stream >> key; return new MessageHardwareKeyReleased(key); LOG_OUT_MSG;}
    static int getID() {if(mID == -1) mID = calcID("MessageHardwareKeyReleased"); return mID;}
    WORD mKey;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG;  stream << mKey << std::endl; LOG_OUT_MSG;}
    static int mID;
};

};

#endif
