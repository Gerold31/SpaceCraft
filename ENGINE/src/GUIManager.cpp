#include "GUIManager.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"

#include "MyGUI_OgrePlatform.h"

#include "ENGINE.hpp"

GUIManager::GUIManager(ENGINE *engine)
{
    mEngine = engine;

    MyGUI::OgrePlatform* mPlatform = new MyGUI::OgrePlatform();
    mPlatform->initialise(engine->getWindow(), engine->getSceneMgr());

    mGUI = new MyGUI::Gui();
    mGUI->initialise();
}

GUIManager::~GUIManager()
{
    mGUI->shutdown();
    delete mGUI;
    mGUI = 0;   
    mPlatform->shutdown();
    delete mPlatform;
    mPlatform = 0;
}

MyGUI::VectorWidgetPtr GUIManager::createGUI(std::string fileName)
{
    MyGUI::VectorWidgetPtr window = MyGUI::LayoutManager::getInstance().loadLayout(fileName);
    return window;
}

void GUIManager::setEnable(bool enable)
{
    mEnable = enable;
    mGUI->setVisiblePointer(enable);
}

bool GUIManager::keyPressed(const OIS::KeyEvent &e)
{
    if(mEnable)
        MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(e.key), e.text);
    return true;
}

bool GUIManager::keyReleased(const OIS::KeyEvent &e)
{
    if(mEnable)
        MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(e.key));
    return true;
}
 
bool GUIManager::mouseMoved(const OIS::MouseEvent &e)
{
    MyGUI::InputManager::getInstance().injectMouseMove(e.state.X.abs, e.state.Y.abs, e.state.Z.abs);
    return true;
}

bool GUIManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if(mEnable)
        MyGUI::InputManager::getInstance().injectMousePress(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
    return true;
}

bool GUIManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if(mEnable)
        MyGUI::InputManager::getInstance().injectMouseRelease(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
    return true;
}