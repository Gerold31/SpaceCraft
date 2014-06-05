#ifndef _MESSAGEUSE_HPP_
#define _MESSAGEUSE_HPP_

#include "Message.hpp"

namespace ENGINE
{

class MessageUse : public Message
{
public:
    MessageUse(std::string user) : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; mUser = user; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; std::string user; stream >> user; LOG_OUT_MSG; return new MessageUse(user); }
    static int getID() {if(mID == -1) mID = calcID("MessageUse"); return mID;}

    std::string mUser;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; stream << mUser << std::endl; LOG_OUT_MSG;}

    static int mID;
};

class MessageRegisterMultiUse : public Message
{
public: // is a MessageRegisterMultiUse sent from client?
    MessageRegisterMultiUse(std::string name) : Message(getID(), true, true, true, true, true) {LOG_IN_MSG; mName = name; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; std::string name; stream >> name; LOG_OUT_MSG; return new MessageRegisterMultiUse(name); }
    static int getID() {if(mID == -1) mID = calcID("MessageRegisterMultiUse"); return mID;}

    std::string mName;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; stream << mName << std::endl; LOG_OUT_MSG;}

    static int mID;
};

class MessageMultiUse : public Message
{
public:
    MessageMultiUse(std::string user, std::string name) : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; mUser = user; mName = name; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; std::string user, name; stream >> user >> name; LOG_OUT_MSG; return new MessageMultiUse(user, name); }
    static int getID() {if(mID == -1) mID = calcID("MessageMultiUse"); return mID;}

    std::string mUser;
    std::string mName;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; stream << mUser << std::endl << mName << std::endl; LOG_OUT_MSG;}

    static int mID;
};

class MessageRequestMultiUse : public Message
{
public:
    MessageRequestMultiUse(std::string user) : Message(getID(), false, true, false, false, false) {LOG_IN_MSG; mUser = user; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; std::string user; stream >> user; LOG_OUT_MSG; return new MessageRequestMultiUse(user); }
    static int getID() {if(mID == -1) mID = calcID("MessageRequestMultiUse"); return mID;}

    std::string mUser;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; stream << mUser << std::endl; LOG_OUT_MSG;}

    static int mID;
};

};

#endif
