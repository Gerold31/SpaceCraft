#include "SpaceShipDesignerGUI.hpp"

#include "ENGINE.hpp"
#include "GUIManager.hpp"
#include "SpaceShipDesigner.hpp"
#include "SpaceShipPart.hpp"

#include <stdlib.h>

SpaceShipDesigner *SpaceShipDesignerGUI::mParent = NULL;

SpaceShipDesignerGUI::SpaceShipDesignerGUI(ENGINE *engine, SpaceShipDesigner *parent)
{
    mParent = parent;
    mEngine = engine;
    mGUI = engine->loadGUI("Designer.layout");
    for(MyGUI::VectorWidgetPtr::iterator i=mGUI.begin(); i!=mGUI.end(); ++i)
    {
        MyGUI::WidgetPtr w = *i;
        if(w->getName() == "Root")
        {
            mRoot = w;
            for(size_t j=0; j<mRoot->getChildCount(); j++)
            {
                if(w->getChildAt(j)->getTypeName() == "Button")
                    w->getChildAt(j)->eventMouseButtonClick = MyGUI::newDelegate(SpaceShipDesignerGUI::buttonClicked);
                else if(w->getChildAt(j)->getTypeName() == "EditBox")
                    ((MyGUI::EditBox *)w->getChildAt(j))->eventEditTextChange = MyGUI::newDelegate(SpaceShipDesignerGUI::editBoxUpdated);
            }
        }
    }
}

void SpaceShipDesignerGUI::setVisible(bool visible)
{
    mRoot->setVisible(visible);
    mEngine->getGUIManager()->setEnable(visible);
}

bool SpaceShipDesignerGUI::getVisible()
{
    return mRoot->getVisible();
}

void SpaceShipDesignerGUI::buttonClicked(MyGUI::Widget *widget)
{
    if(widget->getName() == "SelectFloor")
        mParent->setSelectedPartType(SpaceShipPart::PART_FLOOR);
    else if(widget->getName() == "SelectWall")
        mParent->setSelectedPartType(SpaceShipPart::PART_WALL);
    else if(widget->getName() == "ModeBuild")
        mParent->setMode(SpaceShipDesigner::MODE_BUILD);
    else if(widget->getName() == "ModeWire")
        mParent->setMode(SpaceShipDesigner::MODE_WIRE);
    else if(widget->getName() == "ModeWireLight")
        mParent->setMode(SpaceShipDesigner::MODE_WIRE);
    else if(widget->getName() == "ModeWireDoor")
        mParent->setMode(SpaceShipDesigner::MODE_WIRE);
    else
        mParent->setSelectedPartName(((MyGUI::ButtonPtr)widget)->getCaption().asUTF8());
}

void SpaceShipDesignerGUI::editBoxUpdated(MyGUI::EditBox *widget)
{
    MyGUI::UString caption = widget->getCaption();
    if(widget->getName() == "SelectFloorFrom")
    {
        if(caption == "")
            mParent->enableSelectedFloorFrom(false);
        else
        {
            mParent->setSelectedFloorFrom(atoi(caption.asUTF8().c_str()));
        }
    }
    else if(widget->getName() == "SelectFloorTo")
    {
        if(caption == "")
            mParent->enableSelectedFloorTo(false);
        else
        {
            mParent->setSelectedFloorTo(atoi(caption.asUTF8().c_str()));
        }
    }
}