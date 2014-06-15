#include "ComponentInventory.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "ComponentItem.hpp"
#include "MessageObjectFactory.hpp"
#include "SystemObjectFactory.hpp"
#include "Object.hpp"
#include "ComponentServerConnection.hpp"
#include "SystemServer.hpp"
#include "SystemConfiguration.hpp"

using namespace ENGINE;

TypeInfo *ComponentInventory::mType = new TypeInfo("ComponentInventory", &ComponentInventory::createInstance);

ComponentInventory::ComponentInventory(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
	LOG_IN("component");
    mNumberItems = atoi(boost::any_cast<std::string>(mParams["Slots"]).c_str());
    mItems = new ComponentItem*[mNumberItems];
    LOG_OUT("component");
}

ComponentInventory::~ComponentInventory()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentInventory::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentInventory(object, params);
}

bool ComponentInventory::init()
{
	LOG_IN("component");
    static bool firstTry = true;
    if(firstTry && SystemConfiguration::getSingleton()->isServer())
    {    
        LOG("First try Server, adding Items...", "component");

        ComponentServerConnection *con = (ComponentServerConnection *)mObject->getComponent(ComponentServerConnection::getType());

        if(!con)
        {
            LOG("No ServerConnection", "component");
	        LOG_OUT("component");
            return false;
        }

        SystemLog::getSingleton()->enter("for", "component");
        for(size_t i=0; i<mNumberItems; i++)
        {
            std::string type;
            //debug
            if(rand()%2 == 0)
                type = "ItemEmpty";
            else
                type = "ItemTest";

            char name[64];
            sprintf(name, "%sItemSlot%02X", mObject->getName().c_str(), i);
            LOG(name, "component");
            MessageCreateObject msg(mObject->getSceneNode()->getPosition(), mObject->getSceneNode()->getOrientation(), mObject->getSceneNode()->getParentSceneNode(), name, type + "Server");
            msg.sendTo(SystemObjectFactory::getSingleton());

            msg.mType = type + "Client";
            SystemServer::getSingleton()->sendTo(&msg, SystemObjectFactory::getSingleton(), con);
        }
        SystemLog::getSingleton()->exit("for", "component");
        firstTry = false;
    }

    for(size_t i=0; i<mNumberItems; i++)
    {
        char name[64];
        sprintf(name, "%sItemSlot%02X", mObject->getName().c_str(), i);     

        Object *obj = SystemObjectFactory::getSingleton()->getObject(name);

        if(!obj)
        {
            mReady = false;
            sprintf(name, "%s not found.", name);
            LOG(name, "component");
	        LOG_OUT("component");
            return false;
        }

        mItems[i] = (ComponentItem *)obj->getComponent(ComponentItem::getType());
    }

    mReady = true;
	LOG_OUT("component");
    return true;
}
    
void ComponentInventory::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentInventory::_receiveMessage(Message *message)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}
