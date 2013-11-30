#ifndef _MESSAGEMOVE_HPP_
#define _MESSAGEMOVE_HPP_

#include "Message.hpp"

#include <OIS/OIS.h>

#include <sstream>

namespace ENGINE
{

class MessageStartMoveForward : public Message
{
public:
    MessageStartMoveForward() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStartMoveForward(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStartMoveForward"); return mID;}
private:
    static int mID;
};

class MessageStopMoveForward : public Message
{
public:
    MessageStopMoveForward() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStopMoveForward(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStopMoveForward"); return mID;}
private:
    static int mID;
};

class MessageStartMoveBackward : public Message
{
public:
    MessageStartMoveBackward() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStartMoveBackward(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStartMoveBackward"); return mID;}
private:
    static int mID;
};

class MessageStopMoveBackward : public Message
{
public:
    MessageStopMoveBackward() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStopMoveBackward(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStopMoveBackward"); return mID;}
private:
    static int mID;
};

class MessageStartMoveLeft : public Message
{
public:
    MessageStartMoveLeft() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStartMoveLeft(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStartMoveLeft"); return mID;}
private:
    static int mID;
};

class MessageStopMoveLeft : public Message
{
public:
    MessageStopMoveLeft() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStopMoveLeft(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStopMoveLeft"); return mID;}
private:
    static int mID;
};

class MessageStartMoveRight : public Message
{
public:
    MessageStartMoveRight() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStartMoveRight(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStartMoveRight"); return mID;}
private:
    static int mID;
};

class MessageStopMoveRight : public Message
{
public:
    MessageStopMoveRight() : Message(getID()) {}
    static void *CreateMessage(std::stringstream &stream) { return new MessageStopMoveRight(); }
    static int getID() {if(mID == -1) mID = calcID("MessageStopMoveRight"); return mID;}
private:
    static int mID;
};

class MessageLookAtRel : public Message
{
public:
    MessageLookAtRel(int x, int y) : Message(getID()), mX(x), mY(y) {}
    static void *CreateMessage(std::stringstream &stream) {int x,y; stream >> x >> y; return new MessageLookAtRel(x, y); }
    static int getID() {if(mID == -1) mID = calcID("MessageLookAtRel"); return mID;}
    int mX, mY;
private:
    static int mID;
};

};

#endif
