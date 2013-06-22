#include "Input.hpp"

Input *Input::mInput;
 
Input::Input(void) :
    mMouse(0),
    mKeyboard(0),
    mInputSystem(0) {
}
 
Input::~Input(void) {
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
 
void Input::initialise(Ogre::RenderWindow *renderWindow) {
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
 
void Input::capture(void) {
    // Need to capture / update each device every frame
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
 
void Input::addKeyListener(OIS::KeyListener *keyListener, const std::string &instanceName) {
    if(mKeyboard) {
        // Check for duplicate mems
        mKeyListenerBeg = mKeyListeners.find(instanceName);
        if(mKeyListenerBeg == mKeyListeners.end()) {
            mKeyListeners[instanceName] = keyListener;
        }
        else {
            // Duplicate mem
        }
    }
}
 
void Input::addMouseListener(OIS::MouseListener *mouseListener, const std::string &instanceName) {
    if(mMouse) {
        // Check for duplicate mems
        mMouseListenerBeg = mMouseListeners.find(instanceName);
        if(mMouseListenerBeg == mMouseListeners.end()) {
            mMouseListeners[instanceName] = mouseListener;
        }
        else {
            // Duplicate mem
        }
    }
}
 
void Input::addJoystickListener(OIS::JoyStickListener *joystickListener, const std::string &instanceName) {
    if(mJoysticks.size() > 0) {
        // Check for duplicate mems
        mJoystickListenerBeg = mJoystickListeners.find(instanceName);
        if(mJoystickListenerBeg == mJoystickListeners.end()) {
            mJoystickListeners[instanceName] = joystickListener;
        }
        else {
            // Duplicate mem
        }
    }
}
 
void Input::removeKeyListener(const std::string &instanceName) {
    // Check if mem exists
    mKeyListenerBeg = mKeyListeners.find(instanceName);
    if(mKeyListenerBeg != mKeyListeners.end()) {
        mKeyListeners.erase(mKeyListenerBeg);
    }
    else {
        // Doesn't Exist
    }
}
 
void Input::removeMouseListener(const std::string &instanceName) {
    // Check if mem exists
    mMouseListenerBeg = mMouseListeners.find(instanceName);
    if(mMouseListenerBeg != mMouseListeners.end()) {
        mMouseListeners.erase(mMouseListenerBeg);
    }
    else {
        // Doesn't Exist
    }
}
 
void Input::removeJoystickListener(const std::string &instanceName) {
    // Check if mem exists
    mJoystickListenerBeg = mJoystickListeners.find(instanceName);
    if(mJoystickListenerBeg != mJoystickListeners.end()) {
        mJoystickListeners.erase(mJoystickListenerBeg);
    }
    else {
        // Doesn't Exist
    }
}
 
void Input::removeKeyListener(OIS::KeyListener *keyListener) {
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();
    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        if(mKeyListenerBeg->second == keyListener) {
            mKeyListeners.erase(mKeyListenerBeg);
            break;
        }
    }
}
 
void Input::removeMouseListener(OIS::MouseListener *mouseListener) {
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();
    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        if(mMouseListenerBeg->second == mouseListener) {
            mMouseListeners.erase(mMouseListenerBeg);
            break;
        }
    }
}
 
void Input::removeJoystickListener(OIS::JoyStickListener *joystickListener) {
    mJoystickListenerBeg    = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(mJoystickListenerBeg->second == joystickListener) {
            mJoystickListeners.erase(mJoystickListenerBeg);
            break;
        }
    }
}
 
void Input::removeAllListeners(void) {
    mKeyListeners.clear();
    mMouseListeners.clear();
    mJoystickListeners.clear();
}
 
void Input::removeAllKeyListeners(void) {
    mKeyListeners.clear();
}
 
void Input::removeAllMouseListeners(void) {
    mMouseListeners.clear();
}
 
void Input::removeAllJoystickListeners(void) {
    mJoystickListeners.clear();
}
 
void Input::setWindowExtents(int width, int height) {
    // Set mouse region (if window resizes, we should alter this to reflect as well)
    const OIS::MouseState &mouseState = mMouse->getMouseState();
    mouseState.width  = width;
    mouseState.height = height;
}
 
OIS::Mouse *Input::getMouse(void) {
    return mMouse;
}
 
OIS::Keyboard *Input::getKeyboard(void) {
    return mKeyboard;
}
 
OIS::JoyStick *Input::getJoystick(unsigned int index) {
    // Make sure m's a valid index
    if(index < mJoysticks.size()) {
        return mJoysticks[index];
    }
 
    return 0;
}
 
int Input::getNumOfJoysticks(void) {
    // Cast to keep compiler happy ^^
    return (int) mJoysticks.size();
}
 
bool Input::keyPressed(const OIS::KeyEvent &e) {
    mKeyListenerBeg    = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();
    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        if(!mKeyListenerBeg->second->keyPressed(e))
            break;
    }
 
    return true;
}
 
bool Input::keyReleased(const OIS::KeyEvent &e) {
    mKeyListenerBeg = mKeyListeners.begin();
    mKeyListenerEnd = mKeyListeners.end();
    for(; mKeyListenerBeg != mKeyListenerEnd; ++mKeyListenerBeg) {
        if(!mKeyListenerBeg->second->keyReleased(e))
            break;
    }
 
    return true;
}
 
bool Input::mouseMoved(const OIS::MouseEvent &e) {
    mMouseListenerBeg = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();
    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        if(!mMouseListenerBeg->second->mouseMoved(e))
            break;
    }
 
    return true;
}
 
bool Input::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    mMouseListenerBeg    = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();
    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        if(!mMouseListenerBeg->second->mousePressed(e, id))
            break;
    }
 
    return true;
}
 
bool Input::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) {
    mMouseListenerBeg    = mMouseListeners.begin();
    mMouseListenerEnd = mMouseListeners.end();
    for(; mMouseListenerBeg != mMouseListenerEnd; ++mMouseListenerBeg) {
        if(!mMouseListenerBeg->second->mouseReleased(e, id))
            break;
    }
 
    return true;
}
 
bool Input::povMoved(const OIS::JoyStickEvent &e, int pov) {
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(!mJoystickListenerBeg->second->povMoved(e, pov))
            break;
    }
 
    return true;
}
 
bool Input::axisMoved(const OIS::JoyStickEvent &e, int axis) {
    mJoystickListenerBeg = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(!mJoystickListenerBeg->second->axisMoved(e, axis))
            break;
    }
 
    return true;
}
 
bool Input::sliderMoved(const OIS::JoyStickEvent &e, int sliderID) {
    mJoystickListenerBeg    = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(!mJoystickListenerBeg->second->sliderMoved(e, sliderID))
            break;
    }
 
    return true;
}
 
bool Input::buttonPressed(const OIS::JoyStickEvent &e, int button) {
    mJoystickListenerBeg    = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(!mJoystickListenerBeg->second->buttonPressed(e, button))
            break;
    }
 
    return true;
}
 
bool Input::buttonReleased(const OIS::JoyStickEvent &e, int button) {
    mJoystickListenerBeg    = mJoystickListeners.begin();
    mJoystickListenerEnd = mJoystickListeners.end();
    for(; mJoystickListenerBeg != mJoystickListenerEnd; ++mJoystickListenerBeg) {
        if(!mJoystickListenerBeg->second->buttonReleased(e, button))
            break;
    }
 
    return true;
}
 
Input *Input::getSingletonPtr(void) {
    if(!mInput) {
        mInput = new Input();
    }
 
    return mInput;
}