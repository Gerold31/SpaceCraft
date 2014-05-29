#ifndef _MESSAGEINVENTORY_HPP_
#define _MESSAGEINVENTORY_HPP_

#include "Message.hpp"

namespace ENGINE
{

class MessageEnableInventory : public Message
{
public:
    MessageEnableInventory(bool enable) : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; mEnable = enable; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; bool enable; stream >> enable; LOG_OUT_MSG; return new MessageEnableInventory(enable);}
    static int getID() {if(mID == -1) mID = calcID("MessageEnableInventory"); return mID;}
    bool mEnable;
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; stream << mEnable << std::endl; LOG_OUT_MSG;}
    static int mID;
};

};

#endif
