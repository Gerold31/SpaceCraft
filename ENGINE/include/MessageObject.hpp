#ifndef _MESSAGEOBJECT_HPP_
#define _MESSAGEOBJECT_HPP_

#include "Message.hpp"

namespace ENGINE
{

class MessageEnable : public Message
{
public:
    MessageEnable() : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; LOG_OUT_MSG; return new MessageEnable();}
    static int getID() {if(mID == -1) mID = calcID("MessageEnable"); return mID;}
private:
    static int mID;
};

class MessageDisable : public Message
{
public:
    MessageDisable() : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; LOG_OUT_MSG; return new MessageDisable();}
    static int getID() {if(mID == -1) mID = calcID("MessageDisable"); return mID;}
private:
    static int mID;
};

};

#endif
