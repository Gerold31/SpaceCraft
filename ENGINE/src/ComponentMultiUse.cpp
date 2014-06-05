#include "ComponentMultiUse.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "SystemGUI.hpp"
#include "SystemGraphics.hpp"
#include "SystemConfiguration.hpp"
#include "MessageUse.hpp"
#include "MessageObject.hpp"
#include "Object.hpp"

using namespace ENGINE;

TypeInfo *ComponentMultiUse::mType = new TypeInfo("ComponentMultiUse", &ComponentMultiUse::createInstance);

ComponentMultiUse::ComponentMultiUse(Object *object, ParamMap &params) :
    Component(object, params, mType)
{
    LOG_IN("component");
    mUseList = nullptr;
    LOG_OUT("component");
}

ComponentMultiUse::~ComponentMultiUse()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentMultiUse::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentMultiUse(object, params);
}

bool ComponentMultiUse::init()
{
    LOG_IN("component");
    if(SystemConfiguration::getSingleton()->isClient())
    {
        mUseMenu = SystemGUI::getSingleton()->loadLayout("Use.layout");
        for(size_t i=0; i<mUseMenu.at(0)->getChildCount(); i++)
        {
            MyGUI::Widget *w = mUseMenu.at(0)->getChildAt(i);
            if(w->getTypeName() == "ListBox")
            {
                mUseList = (MyGUI::ListBox *)w;
                break;
            }
        }
        if(!mUseList)
        {
            mReady = true;
            LOG("No List box in layout found!", "error");
            LOG_OUT("component");
            return true;
        }
        mUseList->eventListMouseItemActivate += MyGUI::newDelegate(this, &ComponentMultiUse::eventListClick);

        mUseMenu.at(0)->setSize(SystemGraphics::getSingleton()->getWindow()->getWidth(), SystemGraphics::getSingleton()->getWindow()->getHeight());

        mUseMenu.at(0)->setVisible(false);
    }
    mReady = true;
    LOG_OUT("component");
    return true;
}
    
void ComponentMultiUse::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentMultiUse::receiveMessage(Message *message)
{
    LOG_IN_MSG;
    if(message->getID() == MessageRequestMultiUse::getID())
    {
        if(SystemConfiguration::getSingleton()->isClient())
        {
            LOG("set visible", "component");
            MessageRequestMultiUse *msg = (MessageRequestMultiUse *)message;
            mUser = msg->mUser;
            mUseMenu.at(0)->setVisible(true);
            SystemGUI::getSingleton()->setEnable(true);
        }
    }else if(message->getID() == MessageRegisterMultiUse::getID())
    {
        MessageRegisterMultiUse *msg = (MessageRegisterMultiUse *)message;
        if(SystemConfiguration::getSingleton()->isClient())
        {
            mUseList->addItem(msg->mName);
        }
        if(SystemConfiguration::getSingleton()->isServer())
        {
            mOptions.push_back(msg->mName);
        }
    }else if(message->getID() == MessageSetState::getID())
    {
        LOG("set State", "component");
        if(SystemConfiguration::getSingleton()->isClient()) // should always be true
        {
            while(!mReady)
            {
                boost::this_thread::sleep(boost::posix_time::millisec(10));
            }
            MessageSetState *msg = (MessageSetState *)message;
            for(size_t i=0; i<atoi(msg->mState["Num"].c_str()); i++)
            {
                char key[16];
                sprintf(key, "Item%d", i);
                mUseList->addItem(msg->mState[key]);
            }
        }
        LOG("set State", "component");
    }
    LOG_OUT_MSG;
}

void ComponentMultiUse::eventListClick(MyGUI::ListBox* sender, size_t index)
{
    LOG_IN("component");
    assert(sender == mUseList);
    if(index < sender->getItemCount())
    {
        MessageMultiUse m(mUser, sender->getItemNameAt(index));
        m.sendTo(mObject);
    }
    mUseMenu.at(0)->setVisible(false);
    SystemGUI::getSingleton()->setEnable(false);
    LOG_OUT("component");
}

StateMap ComponentMultiUse::getState()
{
    LOG_IN("component");
    StateMap state;
    char count[16];
    sprintf(count, "%d", mOptions.size());
    LOG(count, "component");
    state["Num"] = count;
    for(size_t i=0; i<mOptions.size(); i++)
    {
        sprintf(count, "Item%d", i);
        state[count] = mOptions.at(i);
        LOG(std::string(count) + " = " + mOptions.at(i), "component");
    }
    LOG_OUT("component");
    return state;
}
