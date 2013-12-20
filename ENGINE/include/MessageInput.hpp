#ifndef _MESSAGEINPUT_HPP_
#define _MESSAGEINPUT_HPP_

#include "Message.hpp"

#include <OIS/OIS.h>

namespace ENGINE
{

class MessageKeyPressed : public Message
{
public:
    MessageKeyPressed(const OIS::KeyEvent &e) : Message(getID(), true, true, false, false, false), mEvent(e) {}
    static int getID() {if(mID == -1) mID = calcID("MessageKeyPressed"); return mID;}

    const OIS::KeyEvent &mEvent;

private:
    static int mID;
};

class MessageKeyReleased : public Message
{
public:
    MessageKeyReleased(const OIS::KeyEvent &e) : Message(getID(), true, true, false, false, false), mEvent(e) {}
    static int getID() {if(mID == -1) mID = calcID("MessageKeyReleased"); return mID;}

    const OIS::KeyEvent &mEvent;

private:
    static int mID;
};

class MessageMouseMoved : public Message
{
public:
    MessageMouseMoved(const OIS::MouseEvent &e) : Message(getID(), true, true, false, false, false), mEvent(e) {}
    static int getID() {if(mID == -1) mID = calcID("MessageMouseMoved"); return mID;}

    const OIS::MouseEvent &mEvent;

private:
    static int mID;
};

class MessageMousePressed : public Message
{
public:
    MessageMousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) : Message(getID(), true, true, false, false, false), mEvent(e), mButtonID(id) {}
    static int getID() {if(mID == -1) mID = calcID("MessageMousePressed"); return mID;}

    const OIS::MouseEvent &mEvent;
    const OIS::MouseButtonID mButtonID;

private:
    static int mID;
};

class MessageMouseReleased : public Message
{
public:
    MessageMouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) : Message(getID(), true, true, false, false, false), mEvent(e), mButtonID(id) {}
    static int getID() {if(mID == -1) mID = calcID("MessageMouseReleased"); return mID;}

    const OIS::MouseEvent &mEvent;
    const OIS::MouseButtonID mButtonID;

private:
    static int mID;
};

};

#endif
