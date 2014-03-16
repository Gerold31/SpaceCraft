#include "SystemGUI.hpp"

#include "SystemLog.hpp"
#include "SystemGraphics.hpp"
#include "Message.hpp"
#include "MessageInput.hpp"

using namespace ENGINE;

SystemGUI::SystemGUI() :
    System("SystemGUI")
{
    LOG_IN("system");
    MyGUI::OgrePlatform* mPlatform = new MyGUI::OgrePlatform();
    mPlatform->initialise(SystemGraphics::getSingleton()->getWindow(), SystemGraphics::getSingleton()->getSceneMgr());

    mGUI = new MyGUI::Gui();
    mGUI->initialise();
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
    if(mEnable)
    {
        if(msg->getID() == MessageKeyPressed::getID())
        {
            MessageKeyPressed *m = (MessageKeyPressed *)msg;
            MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(m->mEvent.key), m->mEvent.text);
        }else if(msg->getID() == MessageKeyReleased::getID())
        {
            MessageKeyReleased *m = (MessageKeyReleased *)msg;
            MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(m->mEvent.key));
        }else if(msg->getID() == MessageMousePressed::getID())
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
    LOG_OUT_MSG;
}

MyGUI::VectorWidgetPtr SystemGUI::createGUI(std::string fileName)
{
    MyGUI::VectorWidgetPtr window = MyGUI::LayoutManager::getInstance().loadLayout(fileName);
    return window;
}

void SystemGUI::setEnable(bool enable)
{
    mEnable = enable;
    MyGUI::PointerManager::getInstancePtr()->setVisible(enable);
}

MyGUI::IntPoint SystemGUI::getMousePos()
{
    return MyGUI::InputManager::getInstancePtr()->getMousePosition();
}
