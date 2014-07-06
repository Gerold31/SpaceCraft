#include "ComponentKeyMapping.hpp"

#include "Object.hpp"
#include "TypeInfo.hpp"
#include "MessageInput.hpp"
#include "MessageMove.hpp"
#include "MessageEngine.hpp"
#include "MessageUse.hpp"
#include "MessageInventory.hpp"
#include "Engine.hpp"
#include "SystemLog.hpp"
#include "SystemInput.hpp"

#include <boost/predef.h>

using namespace ENGINE;

#define LONG_PRESS_TIME (0.5)

#if defined BOOST_OS_WINDOWS && (BOOST_OS_WINDOWS != BOOST_VERSION_NUMBER_NOT_AVAILABLE)
#define OS_STR ("win")
#elif defined BOOST_OS_LINUX && BOOST_OS_LINUX != BOOST_VERSION_NUMBER_NOT_AVAILABLE
#define OS_STR ("linux")
#else
#define OS_STR ("")
#endif

TypeInfo *ComponentKeyMapping::mType = new TypeInfo("ComponentKeyMapping", &ComponentKeyMapping::createInstance);

ComponentKeyMapping::ComponentKeyMapping(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    std::fstream file;

    std::string filename = (boost::any_cast<std::string>(mParams["FileName"]) + "_" + OS_STR + ".cfg");
    file.open(filename.c_str());

    if(file.eof() || !file.is_open())
    {
        file.close();
        LOG("could not open file " + filename, "error");
        LOG_OUT("component");
        return;
    }

    std::string line;

    while(std::getline(file, line))
    {
        char *str = strdup(line.c_str());
        std::string key, event, message;
        key = strtok(str, " \t\n\r");
        event = strtok(NULL, " \t\n\r");
        message = strtok(NULL, " \t\n\r");

        OIS::KeyCode code = SystemInput::getSingleton()->textToKeyCode(key);
        int id =  Message::calcID(message);

        char log[128];
        sprintf(log, "%s: %d, %s, %s", key.c_str(), code, event.c_str(), message.c_str());
        LOG(log, "component");

        if(event == "down")
            mKeyMap[std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(code, DOWN)] = id;
        else if(event == "up")
            mKeyMap[std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(code, UP)] = id;
        else if(event == "press")
        {
            mKeyMap[std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(code, PRESS)] = id;
            mKeyTime[code] = -1.0;
        }else if(event == "longpress")
        {
            mKeyMap[std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(code, LONG_PRESS)] = id;
            mKeyTime[code] = -1.0;
        }
    }

    file.close();
    LOG_OUT("component");
}

ComponentKeyMapping::~ComponentKeyMapping()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentKeyMapping::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentKeyMapping(object, params);
}

bool ComponentKeyMapping::init()
{
    LOG_IN("component");
    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentKeyMapping::update(float elapsedTime)
{
    LOG_IN_FRAME;
    for(auto i=mKeyTime.begin(); i!=mKeyTime.end(); ++i)
    {
        if(i->second >= 0.0)
        {
            i->second += elapsedTime;
            if(i->second > LONG_PRESS_TIME && mKeyMap.count(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(i->first, LONG_PRESS)))
            {
                std::stringstream s;
                Message *msg = Message::createMessage(mKeyMap.at(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(i->first, LONG_PRESS)), s);
                msg->sendTo(mObject);
                delete msg;
                mKeyTime.at(i->first) = -1.0;
            }
        }
    }
    LOG_OUT_FRAME;
}

void ComponentKeyMapping::_receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageMouseMoved::getID())
    {
        MessageMouseMoved *m = (MessageMouseMoved *)message;
        MessageLookAtRel msg(m->mEvent.state.X.rel, m->mEvent.state.Y.rel);
        msg.sendTo(mObject);
    }else if(message->getID() == MessageKeyPressed::getID())
    {
        MessageKeyPressed *m = (MessageKeyPressed *)message;

        if(mKeyMap.count(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, DOWN)))
        {
            std::stringstream s;
            Message *msg = Message::createMessage(mKeyMap.at(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, DOWN)), s);
            msg->sendTo(mObject);
            delete msg;
        }
        if(mKeyTime.count(m->mEvent.key))
        {
            mKeyTime.at(m->mEvent.key) = 0.0;
        }

    }else if(message->getID() == MessageKeyReleased::getID())
    {
        MessageKeyReleased *m = (MessageKeyReleased *)message;

        if(mKeyMap.count(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, UP)))
        {
            std::stringstream s;
            Message *msg = Message::createMessage(mKeyMap.at(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, UP)), s);
            msg->sendTo(mObject);
            delete msg;
        }
        if(mKeyTime.count(m->mEvent.key))
        {
            std::stringstream s;
            if(mKeyTime.at(m->mEvent.key) >= LONG_PRESS_TIME && mKeyMap.count(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, LONG_PRESS)))
            {
                Message *msg = Message::createMessage(mKeyMap.at(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, LONG_PRESS)), s);
                msg->sendTo(mObject);
                delete msg;
            }else if(mKeyMap.count(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, PRESS)))
            {
                Message *msg = Message::createMessage(mKeyMap.at(std::pair<OIS::KeyCode, KEY_EVENT_TYPE>(m->mEvent.key, PRESS)), s);
                msg->sendTo(mObject);
                delete msg;
            }
            mKeyTime.at(m->mEvent.key) = -1.0;
        }
    }else if(message->getID() == MessageMousePressed::getID())
    {
        MessageMousePressed *m = (MessageMousePressed *)message;
        switch(m->mButtonID)
        {
        case OIS::MB_Right:
        {
            MessageUse msg;
            msg.sendTo(mObject);
            break;
        }
        }
    }
    LOG_OUT_MSG;
}
