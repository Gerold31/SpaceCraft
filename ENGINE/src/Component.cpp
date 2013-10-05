#include "Component.hpp"

#include "TypeInfo.hpp"

using namespace ENGINE;

Component::Component(Object *object, std::map<std::string, std::string> params, TypeInfo *type) :
    mObject(object),
    mParams(params),
    mType(type)
{

}
