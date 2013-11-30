#include "SystemInput.hpp"

#include "ComponentKeyboardListener.hpp"
#include "ComponentMouseListener.hpp"
#include "ComponentJoystickListener.hpp"
#include "Object.hpp"
#include "SystemGraphics.hpp"
#include "MessageInput.hpp"

using namespace ENGINE;
 
SystemInput::SystemInput(void) :
    System("SystemInput"),
    mMouse(0),
    mKeyboard(0),
    mInputSystem(0)
{
}
 
SystemInput::~SystemInput(void) {
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
}
 
void SystemInput::init()
{
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
}
 
void SystemInput::update(float elapsedTime)
{
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
}

void SystemInput::receiveMessage(Message *msg)
{
}
 
void SystemInput::addKeyListener(ComponentKeyboardListener *keyListener) {
    if(mKeyboard) {
        mKeyListeners.push_back(keyListener);
    }
}
 
void SystemInput::addMouseListener(ComponentMouseListener *mouseListener) {
    if(mMouse) {
        mMouseListeners.push_back(mouseListener);
    }
}
 
void SystemInput::addJoystickListener(ComponentJoystickListener *joystickListener) {
    if(mJoysticks.size() > 0) {
        mJoystickListeners.push_back(joystickListener);
    }
}
 
void SystemInput::removeKeyListener(ComponentKeyboardListener *keyListener) {
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();
    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        if(*mKeyListenerBeg == keyListener) {
            mKeyListeners.erase(mKeyListenerBeg);
            break;
        }
    }
}
 
void SystemInput::removeMouseListener(ComponentMouseListener *mouseListener) {
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();
    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        if(*mMouseListenerBeg == mouseListener) {
            mMouseListeners.erase(mMouseListenerBeg);
            break;
        }
    }
}
 
void SystemInput::removeJoystickListener(ComponentJoystickListener *joystickListener) {
    mJoystickListenerBeg    = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(*mJoystickListenerBeg == joystickListener) {
            mJoystickListeners.erase(mJoystickListenerBeg);
            break;
        }
    }
}
 
void SystemInput::removeAllListeners() {
    mKeyListeners.clear();
    mMouseListeners.clear();
    mJoystickListeners.clear();
}
 
void SystemInput::removeAllKeyListeners() {
    mKeyListeners.clear();
}
 
void SystemInput::removeAllMouseListeners() {
    mMouseListeners.clear();
}
 
void SystemInput::removeAllJoystickListeners() {
    mJoystickListeners.clear();
}
 
void SystemInput::setWindowExtents(int width, int height) {
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = mMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
}
 
OIS::Mouse *SystemInput::getMouse() {
    return mMouse;
}
 
OIS::Keyboard *SystemInput::getKeyboard() {
    return mKeyboard;
}
 
OIS::JoyStick *SystemInput::getJoystick(unsigned int index) {
    // Make sure m's a valid index
    if(index < mJoysticks.size()) {
        return mJoysticks[index];
    }
 
    return 0;
}
 
int SystemInput::getNumOfJoysticks(void) {
    // Cast to keep compiler happy ^^
    return (int) mJoysticks.size();
}
 
bool SystemInput::keyPressed(const OIS::KeyEvent &e) {
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();

    MessageKeyPressed msg(e);

    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        msg.sendTo((*mKeyListenerBeg)->getObject());
    }
 
    return true;
}
 
bool SystemInput::keyReleased(const OIS::KeyEvent &e) {
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();

    MessageKeyReleased msg(e);

    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        msg.sendTo((*mKeyListenerBeg)->getObject());
    }
 
    return true;
}
 
bool SystemInput::mouseMoved(const OIS::MouseEvent &e) {
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();

    MessageMouseMoved msg(e);

    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        msg.sendTo((*mMouseListenerBeg)->getObject());
    }
 
    return true;
}
 
bool SystemInput::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();

    MessageMousePressed msg(e, id);

    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        msg.sendTo((*mMouseListenerBeg)->getObject());
    }
 
    return true;
}
 
bool SystemInput::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();

    MessageMouseReleased msg(e, id);

    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        msg.sendTo((*mMouseListenerBeg)->getObject());
    }
 
    return true;
}
 
bool SystemInput::povMoved(const OIS::JoyStickEvent &e, int pov) {
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }
 
    return true;
}
 
bool SystemInput::axisMoved(const OIS::JoyStickEvent &e, int axis) {
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }
 
    return true;
}
 
bool SystemInput::sliderMoved(const OIS::JoyStickEvent &e, int sliderID) {
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }
 
    return true;
}
 
bool SystemInput::buttonPressed(const OIS::JoyStickEvent &e, int button) {
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }
 
    return true;
}
 
bool SystemInput::buttonReleased(const OIS::JoyStickEvent &e, int button) {
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
    }
 
    return true;
}
