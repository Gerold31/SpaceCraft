#ifndef _SYSTEMINPUT_HPP_
#define _SYSTEMINPUT_HPP_

#include "Singleton.hpp"
#include "System.hpp"

#include <map>
#include <string>
#include <vector>

#include <OIS/OIS.h> 
#include <OGRE/OgreRenderWindow.h>

namespace ENGINE
{

class ComponentKeyboardListener;
class ComponentMouseListener;
class ComponentJoystickListener;
 
class SystemInput : public System, public Singleton<SystemInput>, public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
friend class Singleton<SystemInput>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);
 
    void addKeyListener(ComponentKeyboardListener *keyListener);
    void addMouseListener(ComponentMouseListener *mouseListener);
    void addJoystickListener(ComponentJoystickListener *joystickListener);
 
    void removeKeyListener(ComponentKeyboardListener *keyListener);
    void removeMouseListener(ComponentMouseListener *mouseListener);
    void removeJoystickListener(ComponentJoystickListener *joystickListener);
 
    void removeAllListeners();
    void removeAllKeyListeners();
    void removeAllMouseListeners();
    void removeAllJoystickListeners();
 
    void setWindowExtents(int width, int height);
 
    OIS::Mouse    *getMouse();
    OIS::Keyboard *getKeyboard();
    OIS::JoyStick *getJoystick(unsigned int index);
 
    int getNumOfJoysticks(void);

private:
    SystemInput();
    SystemInput(const SystemInput&) : System("SystemInput") { }
    ~SystemInput();
 
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
 
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
 
    bool povMoved(const OIS::JoyStickEvent &e, int pov);
    bool axisMoved(const OIS::JoyStickEvent &e, int axis);
    bool sliderMoved(const OIS::JoyStickEvent &e, int sliderID);
    bool buttonPressed(const OIS::JoyStickEvent &e, int button);
    bool buttonReleased(const OIS::JoyStickEvent &e, int button);
 
    OIS::Mouse        *mMouse;
    OIS::Keyboard     *mKeyboard;
    OIS::InputManager *mInputSystem;
 
    std::vector<OIS::JoyStick*> mJoysticks;
    std::vector<OIS::JoyStick*>::iterator mJoystickBeg;
    std::vector<OIS::JoyStick*>::iterator mJoystickEnd;
 
    std::vector<ComponentKeyboardListener*> mKeyListeners;
    std::vector<ComponentMouseListener*> mMouseListeners;
    std::vector<ComponentJoystickListener*> mJoystickListeners;
 
    std::vector<ComponentKeyboardListener*>::iterator mKeyListenerBeg;
    std::vector<ComponentMouseListener*>::iterator mMouseListenerBeg;
    std::vector<ComponentJoystickListener*>::iterator mJoystickListenerBeg;
 
    std::vector<ComponentKeyboardListener*>::iterator mKeyListenerEnd;
    std::vector<ComponentMouseListener*>::iterator mMouseListenerEnd;
    std::vector<ComponentJoystickListener*>::iterator mJoystickListenerEnd;
};

};

#endif
