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
    void receiveMessage(Message *message);

    TypeInfo *getType() {return mType;}
    Object *getObject() {return mObject;}

    bool isReady() {return mReady;}

    virtual StateMap getState() { return StateMap(); }

    boost::any getParam(std::string param) {if(mParams.count(param)) return mParams[param]; return nullptr;}
    std::string getName() { return mName; }

protected:
    virtual void _receiveMessage(Message *message) = 0;
    Object *mObject;
    ParamMap mParams;
    TypeInfo *mType;
    std::string mName;
    bool mReady;
    bool mEnable;
};

};

#endif
