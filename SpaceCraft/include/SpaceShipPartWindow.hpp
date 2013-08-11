#ifndef _SPACESHIPPARTWINDOW_HPP_
#define _SPACESHIPPARTWINDOW_HPP_

#include "SpaceShipPartWall.hpp"

class CPUDoorControl;

class SpaceShipPartWindow : public SpaceShipPartWall
{
public:
    SpaceShipPartWindow(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    SpaceShipPartWindow(SpaceShipPart *old, Ogre::String name);

    bool update(float elapsedTime);

private:
};

#endif
