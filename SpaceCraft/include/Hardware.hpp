#ifndef _HARDWARE_HPP_
#define _HARDWARE_HPP_

#include "SpaceShipPart.hpp"

class CPU;

#include <vector>

typedef unsigned short WORD;
typedef unsigned char  BYTE;

class Hardware : public SpaceShipPart
{
public:
    Hardware(int ID, int MF, int VER, SpaceShipPart::PART_TYPE part, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine);
    Hardware(int ID, int MF, int VER, SpaceShipPart::PART_TYPE part, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, Ogre::String type, ENGINE *engine);
    virtual ~Hardware(){}

    void connect(CPU *_CPU) {mCPU = _CPU;}
    bool isConnected() {return mCPU;}

    virtual bool update(float elapsedTime) = 0;

    virtual void interrupt() = 0;

    WORD getIDL() {return  mID        & 0xFFFF;}
    WORD getIDH() {return (mID >> 16) & 0xFFFF;}
    WORD getVER() {return  mVER;}
    WORD getMFL() {return  mMF        & 0xFFFF;}
    WORD getMFH() {return (mMF >> 16) & 0xFFFF;}

protected:
    CPU *mCPU;
    const unsigned int mID, mMF;
    const WORD mVER;
};

#endif