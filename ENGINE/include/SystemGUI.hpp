#ifndef _SYSTEMGUI_HPP_
#define _SYSTEMGUI_HPP_

#include "System.hpp"

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

namespace ENGINE
{

class SystemGUI : public System, public Singleton<SystemGUI>
{
friend class Singleton<SystemGUI>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    MyGUI::VectorWidgetPtr createGUI(std::string fileName);
    MyGUI::Gui *getGUI() {return mGUI;}

    void setEnable(bool enable);

    MyGUI::IntPoint getMousePos();

private:
    SystemGUI();
    SystemGUI(const SystemGUI &) : System("SystemGUI") {}
    ~SystemGUI();

    MyGUI::OgrePlatform *mPlatform;
    MyGUI::Gui          *mGUI;

    bool mEnable;
};

};

#endif
