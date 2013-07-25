#ifndef _GUIMANAGER_HPP_
#define _GUIMANAGER_HPP_

#include <string>

#include "MyGUI.h"
#include "OIS/OIS.h"

namespace MyGUI
{
    class OgrePlatform;
    class Gui;
}

class ENGINE;

class GUIManager : public OIS::KeyListener, public OIS::MouseListener
{
public:
    GUIManager(ENGINE *engine);
    ~GUIManager();

    MyGUI::VectorWidgetPtr createGUI(std::string fileName);
    MyGUI::Gui *getGUI() {return mGUI;}

    void setEnable(bool enable);

    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
 
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
    MyGUI::OgrePlatform *mPlatform;
    MyGUI::Gui          *mGUI;

    ENGINE              *mEngine;

    bool mEnable;
};

#endif
