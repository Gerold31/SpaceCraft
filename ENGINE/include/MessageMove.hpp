#ifndef _MESSAGEMOVE_HPP_
#define _MESSAGEMOVE_HPP_

#include "Message.hpp"

#include <OIS/OIS.h>

namespace ENGINE
{

class MessageStartMoveForward : public Message
{
public:
    MessageStartMoveForward() : Message() {}
};

class MessageStopMoveForward : public Message
{
public:
    MessageStopMoveForward() {}
};

class MessageStartMoveBackward : public Message
{
public:
    MessageStartMoveBackward() {}
};

class MessageStopMoveBackward : public Message
{
public:
    MessageStopMoveBackward() {}
};

class MessageStartMoveLeft : public Message
{
public:
    MessageStartMoveLeft() {}
};

class MessageStopMoveLeft : public Message
{
public:
    MessageStopMoveLeft() {}
};

class MessageStartMoveRight : public Message
{
public:
    MessageStartMoveRight() {}
};

class MessageStopMoveRight : public Message
{
public:
    MessageStopMoveRight() {}
};

class MessageLookAtRel : public Message
{
public:
    MessageLookAtRel(int x, int y): mX(x), mY(y) {}
    int mX, mY;
};

};

#endif
