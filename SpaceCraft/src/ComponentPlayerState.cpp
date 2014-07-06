#include "ComponentPlayerState.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "MessageObject.hpp"
#include "SystemGraphics.hpp"
#include "Object.hpp"
#include "ComponentMover.hpp"
#include "ComponentKeyMapping.hpp"
#include "ComponentSpaceShipPartPlacer.hpp"
#include "MessageInventory.hpp"

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRay.h>

using namespace SpaceCraft;

TypeInfo *ComponentPlayerState::mType = new TypeInfo("ComponentPlayerState", &ComponentPlayerState::createInstance);

ComponentPlayerState::ComponentPlayerState(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
	LOG_IN("component");
	LOG_OUT("component");
}

ComponentPlayerState::~ComponentPlayerState()
{
	LOG_IN("component");
	LOG_OUT("component");
}

void *ComponentPlayerState::createInstance(Object *object, ParamMap &params)
{
	LOG_IN("component");
	LOG_OUT("component");
    return new ComponentPlayerState(object, params);
}

bool ComponentPlayerState::init()
{
	LOG_IN("component");    
    mReady = true;
    /*
    for(int s=0; s<STATE_COUNT; s++)
    {
        mActiveComponents[(STATE)s] = std::vector<Component *>();
    }*/

    for(size_t i=0; i<mObject->getNumberComponents(); i++)
    {
        Component *c = mObject->getComponent(i);
        if(c->getType() == ComponentMover::getType())
        {
            mActiveComponents[STATE_WALKING].push_back(c);
        }else if(c->getType() == ComponentKeyMapping::getType())
        {
            std::string name = c->getName();
            if(name == "WalkingMapping")
            {
                mActiveComponents[STATE_WALKING].push_back(c);
                mActiveComponents[STATE_BUILDING].push_back(c);
            }else if(name == "InventoryMapping")
            {
                mActiveComponents[STATE_INVENTORY].push_back(c);
            }
        }/*else if(c->getType() == ComponentSpaceShipPartPlacer::getType())
        {
            mActiveComponents[STATE_BUILDING].push_back(c);
        }*/
    }

    for(int s=0; s<STATE_COUNT; s++)
    {
        for(size_t i=0; i<mActiveComponents[(STATE)s].size(); i++)
        {
            MessageDisableComponent m(mActiveComponents[(STATE)s][i]->getName());
            m.sendTo(mObject);
        }
    }
	mState.push_back(STATE_WALKING);
    for(size_t i=0; i<mActiveComponents[STATE_WALKING].size(); i++)
    {
        MessageEnableComponent m(mActiveComponents[STATE_WALKING][i]->getName());
        m.sendTo(mObject);
    }
    LOG_OUT("component");
    return true;
}
    
void ComponentPlayerState::update(float elapsedTime)
{
	LOG_IN_FRAME;
	LOG_OUT_FRAME;
}

void ComponentPlayerState::_receiveMessage(Message *message)
{
	LOG_IN_MSG;
    if(message->getID() == MessageToggleInventory::getID())
    {
        if(mState.back() == STATE_INVENTORY)
        {
            popState();
        }else
        {
            pushState(STATE_INVENTORY);
        }
    }
    /*
    if(message->getID() == MessageKeyPressed::getID())
    {
        MessageKeyPressed *msg = (MessageKeyPressed *)message;
        switch(msg->mEvent.key)
        {
        case OIS::KC_ESCAPE:
			if(mState.front() != STATE_WALKING)
				mState.pop_back();
            break;
        }
    }*/
	LOG_OUT_MSG;
}


void ComponentPlayerState::pushState(STATE s)
{
    LOG_IN("component");
    STATE cur = mState.back();
    for(size_t i=0; i<mActiveComponents[cur].size(); i++)
    {
        MessageDisableComponent m(mActiveComponents[cur][i]->getName());
        m.sendTo(mObject);
    }
	mState.push_back(s);
    for(size_t i=0; i<mActiveComponents[s].size(); i++)
    {
        MessageEnableComponent m(mActiveComponents[s][i]->getName());
        m.sendTo(mObject);
    }
    LOG_OUT("component");
}

void ComponentPlayerState::popState()
{
    LOG_IN("component");
    STATE cur = mState.back();
    for(size_t i=0; i<mActiveComponents[cur].size(); i++)
    {
        MessageDisableComponent m(mActiveComponents[cur][i]->getName());
        m.sendTo(mObject);
    }
	mState.pop_back();
    cur = mState.back();
    for(size_t i=0; i<mActiveComponents[cur].size(); i++)
    {
        MessageEnableComponent m(mActiveComponents[cur][i]->getName());
        m.sendTo(mObject);
    }
    LOG_OUT("component");
}
