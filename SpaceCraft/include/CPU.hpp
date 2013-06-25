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

    WORD getRegister(int r) {return mRegister[r];}
    void setRegister(int r, WORD val) {mRegister[r] = val;}
    WORD *getMemory() {return mRam;}
    void setMemory(Memory *mem);

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
    bool mIdle;

    boost::thread mThread;
    boost::mutex mMutex;

    std::string disas(WORD i);
};

#endif
