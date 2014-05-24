#ifndef _COMPONENT_HPP_
#define _COMPONENT_HPP_

#include "Defines.hpp"

#include "MessageReceiver.hpp"

namespace ENGINE
{

class Message;
class Object;
class TypeInfo;

class Component : public MessageReceiver
{
public:
    Component(Object *object, ParamMap &params, TypeInfo *type);
    virtual ~Component() {};

    virtual bool init() = 0;
    virtual void update(float elapsedTime) = 0;
    virtual void receiveMessage(Message *message) = 0;

    TypeInfo *getType() {return mType;}
    Object *getObject() {return mObject;}

    bool isReady() {return mReady;}

protected:
    Object *mObject;
    ParamMap mParams;
    TypeInfo *mType;
    bool mReady;
};

};

#endif
