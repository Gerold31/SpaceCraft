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

};

#endif
