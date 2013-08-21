#ifndef _CPUDOORCONTROL_HPP_
#define _CPUDOORCONTROL_HPP_

#include "Hardware.hpp"

class SpaceShipPartDoor;

#include <vector>

class CPUDoorControl : public Hardware
{
public:
    CPUDoorControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    CPUDoorControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    void setMaterial();
    
    void interrupt();

    void addDoor(SpaceShipPartDoor *door);
    int getNumberDoors() {return mDoors.size();}
    SpaceShipPartDoor *getDoor(int i) {return mDoors.at(i).first;}

    static std::string getType(){return mType;}

private:
    static std::string mType;

    std::vector<std::pair<SpaceShipPartDoor *, int> >mDoors;
    WORD mInterruptMsg;

    static SpaceShipPartInfo mPartInfo[1];

    void commonConstructor();
};

#endif
