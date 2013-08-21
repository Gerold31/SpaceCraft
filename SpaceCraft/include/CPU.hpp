#ifndef _CPU_HPP_
#define _CPU_HPP_

#include "SpaceShipPart.hpp"

class Hardware;
class Memory;

#include <vector>
#include <queue>
#include <string>

#include "boost/thread.hpp"
#include <boost/date_time.hpp> 

typedef unsigned short WORD;
typedef unsigned char  BYTE;

class CPU : public SpaceShipPart
{
public:
    CPU(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    CPU(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);
    ~CPU();

    bool update(float elapsedTime);
    void setMaterial();

    void interrupt(WORD msg);

    void halt(int cycles){mCycles += cycles;}

    void start();
    void stop();
    void reset();
    void run();

    void addDevice(Hardware *dev);
    int getNumberDevices();
    Hardware *getDevice(int i);

    WORD getRegister(int r) {return mRegister[r];}
    void setRegister(int r, WORD val) {mRegister[r] = val;}
    WORD *getMemory() {return mRam;}
    void setMemory(Memory *mem);

    static std::string getType(){return mType;}

private:
    static std::string mType;

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
    
    static SpaceShipPartInfo mPartInfo[1];

    void commonConstructor();

    std::string disas(WORD i);
};

#endif
