#ifndef _HARDWARE_HPP_
#define _HARDWARE_HPP_

#include "Entity.hpp"

class CPU;

#include <vector>

typedef unsigned short WORD;
typedef unsigned char  BYTE;

class Hardware : public Entity
{
public:
    Hardware(int ID, int MF, int VER, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine);

    void connect(CPU *_CPU) {mCPU = _CPU;}

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