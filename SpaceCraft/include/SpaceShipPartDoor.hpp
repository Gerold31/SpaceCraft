#ifndef _SPACESHIPPARTDOOR_HPP_
#define _SPACESHIPPARTDOOR_HPP_

#include "SpaceShipPartWall.hpp"

class CPUDoorControl;

class SpaceShipPartDoor : public SpaceShipPartWall
{
public:
    SpaceShipPartDoor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    SpaceShipPartDoor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);
    SpaceShipPartDoor(SpaceShipPart *old, Ogre::String name);
    ~SpaceShipPartDoor();

    bool update(float elapsedTime);

    bool isOpen() {return mOpened;}
    bool isLocked() {return mLocked;}

    void open(bool open);
    void lock(bool lock);

    void connect(CPUDoorControl *ctrl) {mControl = ctrl;}
    bool isConnected() {return mControl;}

    static std::string getType(){return mType;}

private:
    static std::string mType;

    Ogre::SceneNode *mDoorNode;
    Ogre::Entity *mDoor;

    CPUDoorControl *mControl;

    bool mOpened;
    bool mLocked;

    void commonConstructor();
};

#endif
