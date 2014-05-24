#include "ComponentItem.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;

TypeInfo *ComponentItem::mType = new TypeInfo("ComponentItem", &ComponentItem::createInstance);

ComponentItem::ComponentItem(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
	LOG_IN("component");
    mName = boost::any_cast<std::string>(mParams["ItemName"]);
    mCount = atoi(boost::any_cast<std::string>(mParams["Count"]).c_str());
	LOG_OUT("component");
}

ComponentItem::~ComponentItem()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentItem::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentItem(object, params);
}

bool ComponentItem::init()
{
	LOG_IN("component");
    mReady = true;
	LOG_OUT("component");
    return true;
}
    
void ComponentItem::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentItem::receiveMessage(Message *message)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

bool ComponentItem::isEmpty() const
{
    return mCount == 0;
}

bool ComponentItem::compare(ComponentItem *other) const
{
    return mName == other->mName;
}

void ComponentItem::move(ComponentItem *other)
{
    //@todo: use message
    other->mCount += mCount;
    other->mName = mName;
    
    mCount = 0;
    mName = "";
}
