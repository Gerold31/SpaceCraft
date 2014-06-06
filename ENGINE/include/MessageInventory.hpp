#ifndef _MESSAGEINVENTORY_HPP_
#define _MESSAGEINVENTORY_HPP_

#include "Message.hpp"

namespace ENGINE
{

class MessageToggleInventory : public Message
{
public:
    MessageToggleInventory() : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) {LOG_IN_MSG; LOG_OUT_MSG; return new MessageToggleInventory();}
    static int getID() {if(mID == -1) mID = calcID("MessageToggleInventory"); return mID;}
private:
    void _serialize(std::ostream &stream) {LOG_IN_MSG; LOG_OUT_MSG;}
    static int mID;
};

};

#endif
