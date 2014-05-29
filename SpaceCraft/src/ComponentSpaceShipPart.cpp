#include "ComponentSpaceShipPart.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace SpaceCraft;

ComponentSpaceShipPart::ComponentSpaceShipPart(Object *object, ParamMap &params, TypeInfo *type, PART_TYPE partType) :
    Component(object, params, type),
    mPartType(partType)
{
	LOG_IN("component");
	LOG_OUT("component");
}

ComponentSpaceShipPart::~ComponentSpaceShipPart()
{
	LOG_IN("component");
	LOG_OUT("component");
}

bool ComponentSpaceShipPart::init()
{
	LOG_IN("component");
    mReady = true;
	LOG_OUT("component");
    return true;
}
    
void ComponentSpaceShipPart::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentSpaceShipPart::_receiveMessage(Message *message)
{
	LOG_IN_MSG;
	LOG_OUT_MSG;
}
