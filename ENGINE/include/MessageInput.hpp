#ifndef _MESSAGEINPUT_HPP_
#define _MESSAGEINPUT_HPP_

#include "Message.hpp"

#include <OIS/OIS.h>

namespace ENGINE
{

class MessageKeyPressed : public Message
{
public:
    MessageKeyPressed(const OIS::KeyEvent &e) : mEvent(e) {}

private:
    const OIS::KeyEvent &mEvent;
};

class MessageKeyReleased : public Message
{
public:
    MessageKeyReleased(const OIS::KeyEvent &e) : mEvent(e) {}

private:
    const OIS::KeyEvent &mEvent;
};

class MessageMouseMoved : public Message
{
public:
    MessageMouseMoved(const OIS::MouseEvent &e) : mEvent(e) {}

private:
    const OIS::MouseEvent &mEvent;
};

class MessageMousePressed : public Message
{
public:
    MessageMousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) : mEvent(e), mID(id) {}

private:
    const OIS::MouseEvent &mEvent;
    const OIS::MouseButtonID mID;
};

class MessageMouseReleased : public Message
{
public:
    MessageMouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) : mEvent(e), mID(id) {}

private:
    const OIS::MouseEvent &mEvent;
    const OIS::MouseButtonID mID;
};

};

#endif
