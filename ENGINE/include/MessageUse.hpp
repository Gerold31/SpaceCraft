#ifndef _MESSAGEUSE_HPP_
#define _MESSAGEUSE_HPP_

#include "Message.hpp"

namespace ENGINE
{

class MessageUse : public Message
{
public:
    MessageUse() : Message(getID(), true, true, true, true, false) {}
    static void *CreateMessage(std::istream &stream) { return new MessageUse(); }
    static int getID() {if(mID == -1) mID = calcID("MessageUse"); return mID;}
private:
    static int mID;
};

};

#endif
