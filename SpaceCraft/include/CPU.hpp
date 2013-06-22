#ifndef _CPU_HPP_
#define _CPU_HPP_

#include "Entity.hpp"

class Hardware;
class Memory;

#include <vector>
#include <queue>
#include <string>

#include "boost/thread.hpp"
#include <boost/date_time.hpp> 

typedef unsigned short WORD;
typedef unsigned char  BYTE;

class CPU : public Entity
{
public:
    CPU(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    void interrupt(WORD msg);

    void halt(int cycles){mCycles += cycles;}

    void start();
    void stop();
    void reset();
    void run();

    void addDevice(Hardware *dev);
    void setMemory(Memory *mem);

    WORD getRegister(int i) {return mRegister[i];}
    WORD *getMemory() {return mRam;}

private:
    const double mFrequency;
    WORD *mRam;
    WORD mRegister[8];
    WORD mPC, mSP, mEX, mIA;
    bool mSkip;

    std::vector<Hardware *> mDevices;

    std::queue<WORD > mInterruptQueue;
    bool mInterrupQueuing;
    int mInstructionsSinceInterrupt;
    int mInterrupts;
    
    int mCycles;
    bool mRunning;

    boost::thread mThread;

    std::string disas(WORD i);
};

#endif