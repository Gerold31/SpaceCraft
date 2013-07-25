#ifndef _SPACESHIPDESIGNERGUI_HPP_
#define _SPACESHIPDESIGNERGUI_HPP_

#include "MyGUI.h"

class ENGINE;
class SpaceShipDesigner;

class SpaceShipDesignerGUI
{
public:
    SpaceShipDesignerGUI(ENGINE *engine, SpaceShipDesigner *parent);

    void setVisible(bool visible);
    bool getVisible();

    static void buttonClicked(MyGUI::Widget *widget);

private:
    ENGINE *mEngine;
    MyGUI::VectorWidgetPtr mGUI;
    MyGUI::WidgetPtr mRoot;

    static SpaceShipDesigner *mParent;
};

#endif