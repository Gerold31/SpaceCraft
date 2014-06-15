#include "SystemGUI.hpp"

#include "SystemLog.hpp"
#include "SystemGraphics.hpp"
#include "Message.hpp"
#include "MessageInput.hpp"

#include "OGRE/OgreRenderWindow.h"

using namespace ENGINE;

SystemGUI::SystemGUI() :
    System("SystemGUI")
{
    LOG_IN("system");
    mPlatform = nullptr;
    mGUI = nullptr;
    mMouseEnable = mKeyboardEnable = false;
    mInit = false;
    LOG_OUT("system");
}

SystemGUI::~SystemGUI()
{
    LOG_IN("system");
    LOG_OUT("system");
}

void SystemGUI::init()
{
    LOG_IN("system");
    LOG_OUT("system");
}
    
void SystemGUI::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void SystemGUI::receiveMessage(Message *msg)
{
    LOG_IN_MSG;
    if(mMouseEnable)
    {
        if(msg->getID() == MessageMousePressed::getID())
        {
            MessageMousePressed *m = (MessageMousePressed *)msg;
            MyGUI::InputManager::getInstance().injectMousePress(m->mEvent.state.X.abs, m->mEvent.state.Y.abs, MyGUI::MouseButton::Enum(m->mButtonID));
        }else if(msg->getID() == MessageMouseReleased::getID())
        {
            MessageMouseReleased *m = (MessageMouseReleased *)msg;
            MyGUI::InputManager::getInstance().injectMouseRelease(m->mEvent.state.X.abs, m->mEvent.state.Y.abs, MyGUI::MouseButton::Enum(m->mButtonID));
        }else if(msg->getID() == MessageMouseMoved::getID())
        {
            MessageMouseMoved *m = (MessageMouseMoved *)msg;
            MyGUI::InputManager::getInstance().injectMouseMove(m->mEvent.state.X.abs, m->mEvent.state.Y.abs, m->mEvent.state.Y.abs);
        }
    }
    if(mKeyboardEnable)
    {
        if(msg->getID() == MessageKeyPressed::getID())
        {
            MessageKeyPressed *m = (MessageKeyPressed *)msg;
            MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(m->mEvent.key), m->mEvent.text);
        }else if(msg->getID() == MessageKeyReleased::getID())
        {
            MessageKeyReleased *m = (MessageKeyReleased *)msg;
            MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(m->mEvent.key));
        }
    }
    LOG_OUT_MSG;
}

MyGUI::VectorWidgetPtr SystemGUI::loadLayout(std::string fileName)
{
    LOG_IN("system");
    LOG_OUT("system");
    return MyGUI::LayoutManager::getInstance().loadLayout(fileName);
}

void SystemGUI::setEnable(bool enable)
{
    LOG_IN_FRAME;
    mMouseEnable = enable;
    mMouseEnable = enable;
    MyGUI::PointerManager::getInstancePtr()->setVisible(enable);
    LOG_OUT_FRAME;
}

void SystemGUI::setMouseEnable(bool enable)
{
    LOG_IN_FRAME;
    mMouseEnable = enable;
    MyGUI::PointerManager::getInstancePtr()->setVisible(enable);
    LOG_OUT_FRAME;
}

void SystemGUI::setKeyboardEnable(bool enable)
{
    LOG_IN_FRAME;
    mKeyboardEnable = enable;
    LOG_OUT_FRAME;
}

MyGUI::IntPoint SystemGUI::getMousePos()
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
    return MyGUI::InputManager::getInstancePtr()->getMousePosition();
}

void SystemGUI::setActiveViewport(unsigned short index)
{
    LOG_IN("system");
    mInit = true;
    if(!mPlatform)
    {
        MyGUI::OgrePlatform* mPlatform = new MyGUI::OgrePlatform();
        mPlatform->initialise(SystemGraphics::getSingleton()->getWindow(), SystemGraphics::getSingleton()->getSceneMgr());

        mGUI = new MyGUI::Gui();
        mGUI->initialise();
        setEnable(false);
    }else
    {
        mPlatform->getRenderManagerPtr()->setActiveViewport(index);
    }
    LOG_OUT("system");
}
