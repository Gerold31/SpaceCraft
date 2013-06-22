#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include <OIS/OIS.h>
 
#include <OGRE/OgreRenderWindow.h>
 
class Input : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener {
public:
    virtual ~Input(void);
 
    void initialise(Ogre::RenderWindow *renderWindow);
    void capture(void);
 
    void addKeyListener(OIS::KeyListener *keyListener, const std::string &instanceName);
    void addMouseListener(OIS::MouseListener *mouseListener, const std::string &instanceName);
    void addJoystickListener(OIS::JoyStickListener *joystickListener, const std::string &instanceName);
 
    void removeKeyListener(const std::string &instanceName);
    void removeMouseListener(const std::string &instanceName);
    void removeJoystickListener(const std::string &instanceName);
 
    void removeKeyListener(OIS::KeyListener *keyListener);
    void removeMouseListener(OIS::MouseListener *mouseListener);
    void removeJoystickListener(OIS::JoyStickListener *joystickListener);
 
    void removeAllListeners(void);
    void removeAllKeyListeners(void);
    void removeAllMouseListeners(void);
    void removeAllJoystickListeners(void);
 
    void setWindowExtents(int width, int height);
 
    OIS::Mouse    *getMouse(void);
    OIS::Keyboard *getKeyboard(void);
    OIS::JoyStick *getJoystick(unsigned int index);
 
    int getNumOfJoysticks(void);
 
    static Input *getSingletonPtr(void);
private:
    Input(void);
    Input(const Input&) { }
    Input  &operator = (const Input&);
 
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
 
    std::map<std::string, OIS::KeyListener*> mKeyListeners;
    std::map<std::string, OIS::MouseListener*> mMouseListeners;
    std::map<std::string, OIS::JoyStickListener*> mJoystickListeners;
 
    std::map<std::string, OIS::KeyListener*>::iterator mKeyListenerBeg;
    std::map<std::string, OIS::MouseListener*>::iterator mMouseListenerBeg;
    std::map<std::string, OIS::JoyStickListener*>::iterator mJoystickListenerBeg;
 
    std::map<std::string, OIS::KeyListener*>::iterator mKeyListenerEnd;
    std::map<std::string, OIS::MouseListener*>::iterator mMouseListenerEnd;
    std::map<std::string, OIS::JoyStickListener*>::iterator mJoystickListenerEnd;
 
    static Input *mInput;
};

#endif