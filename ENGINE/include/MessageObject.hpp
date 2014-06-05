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

class MessageSetState : public Message
{
public:
    MessageSetState(StateMap state) : Message(getID(), true, true, true, true, false) {LOG_IN_MSG; mState = state; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream)
    {
        LOG_IN_MSG;
        StateMap map;
        size_t size;
        stream >> size;
        for(size_t i=0; i<size; i++)
        {
            std::string key, value;
            stream >> key;
            stream >> value;
            map[key] = value;
        }
        LOG_OUT_MSG;
        return new MessageSetState(map);
    }
    static int getID() {if(mID == -1) mID = calcID("MessageSetState"); return mID;}

    StateMap mState;
private:
    void _serialize(std::ostream &stream)
    {
        LOG_IN_MSG;
        stream << mState.size() << std::endl;
        for(auto i = mState.begin(); i!=mState.end(); ++i)
        {
            stream << i->first << std::endl;
            stream << i->second << std::endl;
        }
        LOG_OUT_MSG;
    }

    static int mID;
};

};

#endif
