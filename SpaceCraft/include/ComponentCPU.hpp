#ifndef _COMPONENTCPU_HPP_
#define _COMPONENTCPU_HPP_

#include "SpaceCraftDefines.hpp"
#include "Component.hpp"

#include <queue>
#include <vector>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentHardware;
class ComponentMemory;

class ComponentCPU : public Component
{
public:
    ComponentCPU(Object *object, ParamMap &params);
    ~ComponentCPU();

    static void *createInstance(Object *object, ParamMap &params);

    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    void halt(int cycles){mCycles += cycles;}

    void start();
    void stop();
    void reset();
    void run();

    void addDevice(ComponentHardware *dev) {mDevices.push_back(dev);}
    int getNumberDevices();
    ComponentHardware *getDevice(int i);

    WORD getRegister(int r) {return mRegister[r];}
    void setRegister(int r, WORD val) {mRegister[r] = val;}
    WORD *getMemory() {return mRam;}
    void setMemory(ComponentMemory *mem);

    static TypeInfo *getType() {return mType;}

private:
    void interrupt(WORD msg);
    std::string disas(WORD i);

    static TypeInfo *mType;

    const double mFrequency;
    WORD *mRam;
    WORD mRegister[8];
    WORD mPC, mSP, mEX, mIA;
    bool mSkip;

    std::vector<ComponentHardware *> mDevices;

    std::queue<WORD > mInterruptQueue;
    bool mInterrupQueuing;
    int mInstructionsSinceInterrupt;
    int mInterrupts;

    int mCycles;
    bool mRunning;
    bool mIdle;

    boost::thread mThread;
    boost::mutex mMutex;

};

};

#endif
