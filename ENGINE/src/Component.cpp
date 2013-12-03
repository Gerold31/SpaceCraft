#include "Component.hpp"

#include "TypeInfo.hpp"

using namespace ENGINE;

Component::Component(Object *object, ParamMap &params, TypeInfo *type) :
    MessageReceiver(MessageReceiver::RECEIVER_COMPONENT),
    mObject(object),
    mParams(params),
    mType(type)
{

}
