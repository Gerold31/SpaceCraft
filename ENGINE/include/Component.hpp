#ifndef _COMPONENT_HPP_
#define _COMPONENT_HPP_

#include "MessageReceiver.hpp"

#include <map>
#include <string>

namespace ENGINE
{

class Message;
class Object;
class TypeInfo;

class Component : public MessageReceiver
{
public:
    Component(Object *object, std::map<std::string, std::string> params, TypeInfo *type);
    virtual ~Component() {};

    virtual void update(float elapsedTime) = 0;
    virtual void receiveMessage(Message *message) = 0;

    TypeInfo *getType() {return mType;}
    Object *getObject() {return mObject;}

protected:
    Object *mObject;
    std::map<std::string, std::string>  mParams;
    TypeInfo *mType;
};

};

#endif
