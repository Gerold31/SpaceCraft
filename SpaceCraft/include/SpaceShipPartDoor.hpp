#ifndef _SPACESHIPPARTDOOR_HPP_
#define _SPACESHIPPARTDOOR_HPP_

#include "SpaceShipPartWall.hpp"

class SpaceShipPartDoor : public SpaceShipPartWall
{
public:
    SpaceShipPartDoor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    SpaceShipPartDoor(SpaceShipPart *old, Ogre::String name);

    bool update(float elapsedTime);

    bool isOpen() {return mOpened;}
    bool isLocked() {return mLocked;}

    void open(bool open);
    void lock(bool lock);

private:
    Ogre::SceneNode *mDoorNode;
    Ogre::Entity *mDoor;

    bool mOpened;
    bool mLocked;
};

#endif
