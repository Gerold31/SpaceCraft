#ifndef _CPUDOORCONTROL_HPP_
#define _CPUDOORCONTROL_HPP_

#include "Hardware.hpp"

class SpaceShipPartDoor;

#include <vector>

class CPUDoorControl : public Hardware
{
public:
    CPUDoorControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void interrupt();

    void addDoor(SpaceShipPartDoor *door);
    int getNumberDoors() {return mDoors.size();}
    SpaceShipPartDoor *getDoor(int i) {return mDoors.at(i).first;}

private:
    std::vector<std::pair<SpaceShipPartDoor *, int> >mDoors;
    WORD mInterruptMsg;

    static SpaceShipPartInfo mPartInfo[1];
};

#endif
