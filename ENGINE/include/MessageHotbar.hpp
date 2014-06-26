#ifndef _MESSAGEHOTBAR_HPP_
#define _MESSAGEHOTBAR_HPP_

#include "Message.hpp"

namespace ENGINE
{

class MessageNextSlot : public Message
{
public:
    MessageNextSlot() : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; LOG_OUT_MSG; return new MessageNextSlot();}
    static int getID() {if(mID == -1) mID = calcID("MessageNextSlot"); return mID;}
private:
    static int mID;
};

class MessagePrevSlot : public Message
{
public:
    MessagePrevSlot() : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; LOG_OUT_MSG; return new MessagePrevSlot();}
    static int getID() {if(mID == -1) mID = calcID("MessagePrevSlot"); return mID;}
private:
    static int mID;
};

class MessageSelectHotbarItem : public Message
{
public:
    MessageSelectHotbarItem(std::string name) : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; mName = name; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; std::string name; stream >> name; LOG_OUT_MSG; return new MessageSelectHotbarItem(name);}
    static int getID() {if(mID == -1) mID = calcID("MessageSelectHotbarItem"); return mID;}
    std::string mName;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; stream << mName << std::endl; LOG_OUT_MSG;}
    static int mID;
};

#define MESSAGESLOTNAME(i) MessageSlot##i
#define PRIMITIVE_STR(x) #x
#define STR(x) PRIMITIVE_STR(x)
#define MESSAGESLOTSTR(i) STR(MESSAGESLOTNAME(i))

#define MESSAGESLOT(i) \
class MESSAGESLOTNAME(i) : public Message\
{\
public:\
    MESSAGESLOTNAME(i)() : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; LOG_OUT_MSG;}\
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; LOG_OUT_MSG; return new MESSAGESLOTNAME(i)();}\
    static int getID() {if(mID == -1) mID = calcID(MESSAGESLOTSTR(i)); return mID;}\
private:\
    static int mID;\
};\

MESSAGESLOT(0);
MESSAGESLOT(1);
MESSAGESLOT(2);
MESSAGESLOT(3);
MESSAGESLOT(4);
MESSAGESLOT(5);
MESSAGESLOT(6);
MESSAGESLOT(7);
MESSAGESLOT(8);
MESSAGESLOT(9);

};

#endif
