#include "SystemInput.hpp"

#include "ComponentKeyboardListener.hpp"
#include "ComponentMouseListener.hpp"
#include "ComponentJoystickListener.hpp"
#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "MessageInput.hpp"
#include "SystemLog.hpp"

using namespace ENGINE;
 
SystemInput::SystemInput(void) :
    System("SystemInput"),
    mMouse(0),
    mKeyboard(0),
    mInputSystem(0)
{
    LOG_IN("system");
    LOG_OUT("system");
}
 
SystemInput::~SystemInput(void)
{
    LOG_IN("system");
    if(mInputSystem) {
        if(mMouse) {
            mInputSystem->destroyInputObject(mMouse);
            mMouse = 0;
        }
 
        if(mKeyboard) {
            mInputSystem->destroyInputObject(mKeyboard);
            mKeyboard = 0;
        }
 
        if(mJoysticks.size() > 0) {
            mJoystickBeg = mJoysticks.begin();
            mJoystickEnd = mJoysticks.end();
            for(; mJoystickBeg != mJoystickEnd; ++mJoystickBeg) {
                mInputSystem->destroyInputObject(*mJoystickBeg);
            }
 
            mJoysticks.clear();
        }
 
        // If you use OIS1.0RC1 or above, uncomment this line
        // and comment the line below m
        mInputSystem->destroyInputSystem(mInputSystem);
        //mInputSystem->destroyInputSystem();
        mInputSystem = 0;
 
        // Clear Listeners
        mKeyListeners.clear();
        mMouseListeners.clear();
        mJoystickListeners.clear();
    }
    LOG_OUT("system");
}
 
void SystemInput::init()
{
    LOG_IN("system");
    Ogre::RenderWindow *renderWindow = SystemGraphics::getSingleton()->getWindow();
    if(!mInputSystem) {
        // Setup basic variables
        OIS::ParamList paramList;    
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
 
        // Get window handle
        renderWindow->getCustomAttribute("WINDOW", &windowHnd);
 
        // Fill parameter list
        windowHndStr << (unsigned int) windowHnd;
        paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
        // Create inputsystem
        mInputSystem = OIS::InputManager::createInputSystem(paramList);
 
        // If possible create a buffered keyboard
        // (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
        //if(mInputSystem->numKeyboards() > 0) {
        if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
            mKeyboard = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject(OIS::OISKeyboard, true));
            mKeyboard->setEventCallback(this);
        }
 
        // If possible create a buffered mouse
        // (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
        //if(mInputSystem->numMice() > 0) {
        if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
            mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject(OIS::OISMouse, true));
            mMouse->setEventCallback(this);
 
            // Get window size
            unsigned int width, height, depth;
            int left, top;
            renderWindow->getMetrics(width, height, depth, left, top);
 
            // Set mouse region
            this->setWindowExtents(width, height);
        }
 
        // If possible create all joysticks in buffered mode
        // (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISJoyStick) > 0) {
        //if(mInputSystem->numJoysticks() > 0) {
        if (mInputSystem->getNumberOfDevices(OIS::OISJoyStick) > 0) {
            //mJoysticks.resize(mInputSystem->numJoysticks());
            mJoysticks.resize(mInputSystem->getNumberOfDevices(OIS::OISJoyStick));
 
            mJoystickBeg = mJoysticks.begin();
            mJoystickEnd = mJoysticks.end();
            for(; mJoystickBeg != mJoystickEnd; ++mJoystickBeg) {
                (*mJoystickBeg) = static_cast<OIS::JoyStick*>(mInputSystem->createInputObject(OIS::OISJoyStick, true));
                (*mJoystickBeg)->setEventCallback(this);
            }
        }
    }
    LOG_OUT("system");
}
 
void SystemInput::update(float elapsedTime)
{
    LOG_IN_FRAME;
    if(mMouse) {
        mMouse->capture();
    }
 
    if(mKeyboard) {
        mKeyboard->capture();
    }
 
    if(mJoysticks.size() > 0) {
        mJoystickBeg = mJoysticks.begin();
        mJoystickEnd = mJoysticks.end();
        for(; mJoystickBeg != mJoystickEnd; ++mJoystickBeg) {
            (*mJoystickBeg)->capture();
        }
    }
    LOG_OUT_FRAME;
}

void SystemInput::receiveMessage(Message *msg)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}
 
void SystemInput::addKeyListener(ComponentKeyboardListener *keyListener)
{
    LOG_IN("system");
    if(mKeyboard) {
        mKeyListeners.push_back(keyListener);
    }
    LOG_OUT("system");
}
 
void SystemInput::addMouseListener(ComponentMouseListener *mouseListener)
{
    LOG_IN("system");
    if(mMouse) {
        mMouseListeners.push_back(mouseListener);
    }
    LOG_OUT("system");
}
 
void SystemInput::addJoystickListener(ComponentJoystickListener *joystickListener)
{
    LOG_IN("system");
    if(mJoysticks.size() > 0) {
        mJoystickListeners.push_back(joystickListener);
    }
    LOG_OUT("system");
}
 
void SystemInput::removeKeyListener(ComponentKeyboardListener *keyListener)
{
    LOG_IN("system");
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();
    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        if(*mKeyListenerBeg == keyListener) {
            mKeyListeners.erase(mKeyListenerBeg);
            break;
        }
    }
    LOG_OUT("system");
}
 
void SystemInput::removeMouseListener(ComponentMouseListener *mouseListener)
{
    LOG_IN("system");
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();
    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        if(*mMouseListenerBeg == mouseListener) {
            mMouseListeners.erase(mMouseListenerBeg);
            break;
        }
    }
    LOG_OUT("system");
}
 
void SystemInput::removeJoystickListener(ComponentJoystickListener *joystickListener)
{
    LOG_IN("system");
    mJoystickListenerBeg    = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(*mJoystickListenerBeg == joystickListener) {
            mJoystickListeners.erase(mJoystickListenerBeg);
            break;
        }
    }
    LOG_OUT("system");
}
 
void SystemInput::removeAllListeners()
{
    LOG_IN("system");
    mKeyListeners.clear();
    mMouseListeners.clear();
    mJoystickListeners.clear();
    LOG_OUT("system");
}
 
void SystemInput::removeAllKeyListeners()
{
    LOG_IN("system");
    mKeyListeners.clear();
    LOG_OUT("system");
}
 
void SystemInput::removeAllMouseListeners()
{
    LOG_IN("system");
    mMouseListeners.clear();
    LOG_OUT("system");
}
 
void SystemInput::removeAllJoystickListeners()
{
    LOG_IN("system");
    mJoystickListeners.clear();
    LOG_OUT("system");
}
 
void SystemInput::setWindowExtents(int width, int height)
{
    LOG_IN("system");
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = mMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
    LOG_OUT("system");
}
 
OIS::Mouse *SystemInput::getMouse()
{
    LOG_IN("system");
    LOG_OUT("system");
    return mMouse;
}
 
OIS::Keyboard *SystemInput::getKeyboard()
{
    LOG_IN("system");
    LOG_OUT("system");
    return mKeyboard;
}
 
OIS::JoyStick *SystemInput::getJoystick(unsigned int index)
{
    LOG_IN("system");
    // Make sure m's a valid index
    if(index < mJoysticks.size()) {
        LOG_OUT("system");
        return mJoysticks[index];
    }

    LOG_OUT("system");
    return 0;
}
 
int SystemInput::getNumOfJoysticks(void)
{
    LOG_IN("system");
    LOG_OUT("system");
    // Cast to keep compiler happy ^^
    return (int) mJoysticks.size();
}
 
bool SystemInput::keyPressed(const OIS::KeyEvent &e)
{
    LOG_IN_FRAME;
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();

    MessageKeyPressed msg(e);

    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        msg.sendTo((*mKeyListenerBeg)->getObject());
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::keyReleased(const OIS::KeyEvent &e)
{
    LOG_IN_FRAME;
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();

    MessageKeyReleased msg(e);

    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        msg.sendTo((*mKeyListenerBeg)->getObject());
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::mouseMoved(const OIS::MouseEvent &e)
{
    LOG_IN_FRAME;
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();

    MessageMouseMoved msg(e);

    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        msg.sendTo((*mMouseListenerBeg)->getObject());
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    LOG_IN_FRAME;
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();

    MessageMousePressed msg(e, id);

    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        msg.sendTo((*mMouseListenerBeg)->getObject());
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    LOG_IN_FRAME;
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();

    MessageMouseReleased msg(e, id);

    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        msg.sendTo((*mMouseListenerBeg)->getObject());
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::povMoved(const OIS::JoyStickEvent &e, int pov)
{
    LOG_IN_FRAME;
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::axisMoved(const OIS::JoyStickEvent &e, int axis)
{
    LOG_IN_FRAME;
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
{
    LOG_IN_FRAME;
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::buttonPressed(const OIS::JoyStickEvent &e, int button)
{
    LOG_IN_FRAME;
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }

    LOG_OUT_FRAME;
    return true;
}
 
bool SystemInput::buttonReleased(const OIS::JoyStickEvent &e, int button)
{
    LOG_IN_FRAME;
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }

    LOG_OUT_FRAME;
    return true;
}
