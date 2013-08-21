#include "CPU.hpp"

#include "ENGINE.hpp"
#include "Map.hpp"

#include "Hardware.hpp"
#include "Memory.hpp"

#include <assert.h>

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPartInfo CPU::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, -0.75, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

std::string CPU::mType = "CPU";

CPU::CPU(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_FLOORMOUNT, true, pos, ori, parent, name, mType, engine),
    mFrequency(1000000.0)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPU.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    commonConstructor();
}

CPU::CPU(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_FLOORMOUNT, true, pos, ori, parent, staticGeometry, name, mType, engine),
    mFrequency(1000000.0)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPU.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);
    
    commonConstructor();
}

CPU::~CPU()
{
    stop();
}

bool CPU::update(float elapsedTime)
{
    /*
    if(mInstructions >= 1000)
    {
        printf("cycles/second: %f\n", mInstructions/mTime);
        mInstructions = 0;
        mTime = 0;
    }*/
    return true;
}

void CPU::commonConstructor()
{
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }

    mRunning = false;
    mInterrupQueuing = false;
    mInstructionsSinceInterrupt = 0;
    mInterrupts = 0;
    reset();
}

void CPU::interrupt(WORD msg)
{
    if(mIA != 0)
    {
        if(mIdle)
        {
            mIdle = false;
            mThread.interrupt();
        }
        if(mInterrupQueuing)
        {
            if(mInterrupts < 0x100)
            {
                mInterruptQueue.push(msg);
                mInterrupts++;
            }else
                ; // @todo catch fire
        }else
        {
            mMutex.lock();
            mRam[--mSP] = mPC;          // PUSH PC
            mRam[--mSP] = mRegister[0]; // PUSH A
            mPC = mIA;                  // mov PC, IA
            mRegister[0] = msg;         // mov A , a
            mMutex.unlock();
        }
    }
}

void CPU::setMemory(Memory *mem)
{
    mRam = mem->getMemory();
}

void CPU::start()
{
    mRunning = true;
    mThread = boost::thread(boost::bind(&CPU::run, this));
}

void CPU::stop()
{
    mRunning = false;
    mThread.join();
}

void CPU::reset()
{
    for(int i=0; i<8; i++)
        mRegister[i] = 0;
    mPC = mSP = mEX = mIA = 0;
    mCycles = 100; // startup time;
    mSkip = false;
    while(!mInterruptQueue.empty())
        mInterruptQueue.pop();
    mInterrupts = 0;
    mIdle = false;
}

void CPU::run()
{
    unsigned int cycle = 0;
    unsigned int instr = 0;
    boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::local_time();

    WORD i;
    BYTE op;
    WORD a;
    WORD b; 
    WORD *pa;
    WORD *pb;

    while(mRunning /*&& cycle < 1000000000*/)
    {
        instr++;
        cycle += mCycles;
        
        if(mIdle)
        {
            try{
                while(mRunning)
                    boost::this_thread::sleep(boost::posix_time::seconds(1));
                break;
            }catch(...)
            {
            }
        }

        mMutex.lock();
        
        // process interruptqueue
        if(mInterruptQueue.size() > 0 && !mInterrupQueuing && mInstructionsSinceInterrupt > 0)
        {
            if(mIA != 0)
            {
                mInterrupQueuing = true;
                mRam[--mSP] = mPC;                      // PUSH PC
                mRam[--mSP] = mRegister[0];             // PUSH A
                mPC = mIA;                              // mov PC, IA
                mRegister[0] = mInterruptQueue.front(); // mov A , a
                mInterruptQueue.pop();
                mInterrupts--;
            }else
            {
                while(mInterruptQueue.size());
                    mInterruptQueue.pop();
                mInterrupts = 0;
            }
            mCycles=4;
            mInstructionsSinceInterrupt = 0;

            mMutex.unlock();
            continue;
        }
        mInstructionsSinceInterrupt++; // or = 1?
        
        mCycles = 0;
        i = mRam[mPC++];
        op =  i          & 0x1F;
        a  =  i >> 0x0A;
        b  = (i >> 0x05) & 0x1F;
        pa = pb = NULL;

        if(op != 0x00)
        {
            switch(b)
            {
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                pb = &mRegister[b];
                break;
            case 0x08:
            case 0x09:
            case 0x0A:
            case 0x0B:
            case 0x0C:
            case 0x0D:
            case 0x0E:
            case 0x0F:
                pb = &mRam[mRegister[b-0x08]];
                break;
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
                pb = &mRam[mRegister[b-0x10] + mRam[mPC++]];
                mCycles++;
                break;
            case 0x18:
                pb = &mRam[--mSP];
            case 0x19:
                pb = &mRam[mSP];
                break;
            case 0x1A:
                pb = &mRam[mSP + mRam[mPC++]];
                mCycles++;
                break;
            case 0x1B:
                pb = &mSP;
                break;
            case 0x1C:
                pb = &mPC;
                break;
            case 0x1D:
                pb = &mEX;
                break;
            case 0x1E:
                pb = &mRam[mRam[mPC++]];
                mCycles++;
                break;
            default:
                ;
            }
        }

        switch(a)
        {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            pa = &mRegister[a];
            break;
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
            pa = &mRam[mRegister[a-0x08]];
            break;
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
            pa = &mRam[mRegister[a-0x10] + mRam[mPC++]];
            mCycles++;
            break;
        case 0x18:
            if(op == 0 && (b == 0x09 || b == 0x10))
                pb = &mRam[--mSP];
            else
                pa = &mRam[mSP++];
            break;
        case 0x19:
            pa = &mRam[mSP];
            break;
        case 0x1A:
            pa = &mRam[mSP + mRam[mPC++]];
            mCycles++;
            break;
        case 0x1B:
            pa = &mSP;
            break;
        case 0x1C:
            pa = &mPC;
            break;
        case 0x1D:
            pa = &mEX;
            break;
        case 0x1E:
            pa = &mRam[mRam[mPC++]];
            mCycles++;
            break;
        case 0x1F:
            a = mRam[mPC++];
            mCycles++;
            break;
        default:
            a = a - 0x21;
        }

        if(pa)
            a = (*pa);

    
        if(mSkip)
        {
            if(op < 0x10 || op > 0x17) 
                mSkip = false;          // no more if's
            mCycles=1;
            //printf("skip: PC: %4x op: %4x %s\n", mPC, i, disas(i).c_str());

            mMutex.unlock();
            continue;
        }

        //printf("      PC: %4x op: %4x %s\n", mPC, i, disas(i).c_str());
    

        switch(op)
        {
            case 0x00: // special
            {
                switch(b)
                {
                    case 0x01: // JSR a
                    {
                        mRam[--mSP] = mPC; // PUSH PC
                        mPC = a;
                        mCycles+=3;
                        break;
                    }
                    case 0x08: // INT a
                    {
                        if(mIA != 0)
                        {
                            if(mInterrupQueuing)
                            {
                                if(mInterrupts < 0x100)
                                {
                                    mInterruptQueue.push(a);
                                    mInterrupts++;
                                }else
                                    ; // @todo catch fire
                            }else
                            {
                                mInterrupQueuing = true;
                                mRam[--mSP] = mPC;          // PUSH PC
                                mRam[--mSP] = mRegister[0]; // PUSH A
                                mPC = mIA;                  // mov PC, IA
                                mRegister[0] = a;           // mov A , a
                            }
                        }
                        mCycles+=4;
                        break;
                    }
                    case 0x09: // IAG a
                    {
                        if(pa)
                            (*pa) = mIA;
                        mCycles+=1;
                        break;
                    }
                    case 0x0A: // IAS a
                    {
                        mIA = a;
                        if(!mIA)
                        {
                            while(mInterruptQueue.size());
                                mInterruptQueue.pop();
                            mInterrupts = 0;
                        }
                        mCycles+=1;
                        break;
                    }
                    case 0x0B: // RFI a
                    {
                        mInterrupQueuing = false;
                        mRegister[0] = mRam[mSP++]; // POP A
                        mPC = mRam[mSP++];          // POP PC
                        mCycles+=3;
                        break;
                    }
                    case 0x0C: // IAQ a
                    {
                        if(a)
                            mInterrupQueuing = true;
                        else
                            mInterrupQueuing = false;
                        mCycles+=2;
                        break;
                    }
                    case 0x10: // HWN a
                    {
                        (*pa) = mDevices.size();
                        mCycles+=2;
                        break;
                    }
                    case 0x11: // HWQ a
                    {
                        Hardware *dev = mDevices.at(a);
                        if(dev)
                        {
                            mRegister[0] = dev->getIDL();
                            mRegister[1] = dev->getIDH();
                            mRegister[2] = dev->getVER();
                            mRegister[3] = dev->getMFL();
                            mRegister[4] = dev->getMFH();
                        }else
                        {
                            mRegister[0] = mRegister[1] = mRegister[2] = mRegister[3] = mRegister[4] = 0;
                        }
                        mCycles+=4;
                        break;
                    }
                    case 0x12: // HWI a
                    {
                        Hardware *dev = mDevices.at(a);
                        if(dev)
                            dev->interrupt();
                        mCycles+=4;
                        break;
                    }
                    case 0x1F: // WFI
                    {
                        mIdle = true;
                        if(a == mRam[mPC])
                            mPC--;  // 2 Word instruction
                        mPC--;
                        mCycles++;
                        break;
                    }
                default:
                    break;
                }
                break;
            }
            case 0x01: // SET b, a
            {
                (*pb) = a;
                mCycles++;
                break;
            }
            case 0x02: // ADD b, a
            {
                if((*pb) + a > 0xFFFF)
                    mEX = 0x1;
                else
                    mEX = 0x0;
                (*pb) += a;
                mCycles+=2;
                break;
            }
            case 0x03: // SUB b, a
            {
                if((signed)((*pb) - a) < 0)
                    mEX = 0xFFFF;
                else
                    mEX = 0x0;
                (*pb) -= a;
                mCycles+=2;
                break;
            }
            case 0x04: // MUL b, a
            {
                unsigned int t = (*pb) * a;
                (*pb) = t      & 0xFFFF;
                mEX   =(t>>16) & 0xFFFF;
                mCycles+=2;
                break;
            }
            case 0x05: // MLI b, a
            {
                int t = (*pb) * a;
                (*pb) = t      & 0xFFFF;
                mEX   =(t>>16) & 0xFFFF;
                mCycles+=2;
                break;
            }
            case 0x06: // DIV b, a
            {
                if(a==0)
                    (*pb) = mEX = 0;
                else
                {
                    mEX   = (((*pb)<<16)/a) & 0xFFFF;
                    (*pb) = ( (*pb)     /a) & 0xFFFF;
                }
                mCycles+=3;
                break;
            }
            case 0x07: // DVI b, a
            {
                if(a==0)
                    (*pb) = mEX = 0;
                else
                {
                    mEX   = (signed)(((*pb)<<16)/a) & 0xFFFF;
                    (*pb) = (signed)( (*pb)     /a) & 0xFFFF;
                }
                mCycles+=3;
                break;
            }
            case 0x08: // MOD b, a
            {
                if(a==0)
                    (*pb) = 0;
                else (*pb) %= a;
                mCycles+=3;
                break;
            }
            case 0x09: // MDI b, a
            {
                if(a==0)
                    (*pb) = 0;
                else (signed)((*pb) %= a);
                mCycles+=3;
                break;
            }
            case 0x0A: // AND b, a
            {
                (*pb) &= a;
                mCycles+=1;
                break;
            }
            case 0x0B: // BOR b, a
            {
                (*pb) |= a;
                mCycles+=1;
                break;
            }
            case 0x0C: // XOR b, a
            {
                (*pb) ^= a;
                mCycles+=1;
                break;
            }
            case 0x0D: // SHR b, a
            {
                (*pb) = (unsigned)(*pb)>>a;
                mCycles+=1;
                break;
            }
            case 0x0E: // ASR b, a
            {
                (*pb) = (signed)(*pb)>>a;
                mCycles+=1;
                break;
            }
            case 0x0F: // SHL b, a
            {
                (*pb) = (*pb)<<a;
                mCycles+=1;
                break;
            }
            case 0x10: // IFB b, a
            {
                if(((*pb) & a)== 0)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x11: // IFC b, a
            {
                if(((*pb) & a)!= 0)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x12: // IFE b, a
            {
                if((*pb) != a)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x13: // IFN b, a
            {
                if((*pb) == a)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x14: // IFG b, a
            {
                if((*pb) <= a)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x15: // IFA b, a
            {
                if((signed)(*pb) <= (signed)a)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x16: // IFL b, a
            {
                if((*pb) >= a)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x17: // IFU b, a
            {
                if((signed)(*pb) >= (signed)a)
                    mSkip = true;
                mCycles+=2;
                break;
            }
            case 0x1A: // ADX b, a
            {
                if((*pb) + a + mEX > 0xFFFF)
                    mEX = 0x1;
                else
                    mEX = 0x0;
                (*pb) += a + mEX;
                mCycles+=3;
                break;
            }
            case 0x1B: // SBX b, a 
            {
                if((signed)((*pb) - a - mEX) < 0)
                    mEX = 0xFFFF;
                else
                    mEX = 0x0;
                (*pb) -= (a + mEX);
                mCycles+=3;
                break;
            }
            case 0x1E: // STI b, a
            {
                (*pb) = a;
                mRegister[6]++;
                mRegister[7]++;
                mCycles+=2;
                break;
            }
            case 0x1F: // STD b, a
            {
                (*pb) = a;
                mRegister[6]--;
                mRegister[7]--;
                mCycles+=2;
                break;
            }
        default:
            break;
        }

        mMutex.unlock();
    }
    boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration diff = t1 - t0;
    printf("MHZ:  %f\n", 1.0*cycle/diff.total_microseconds());
    printf("MIPS: %f\n", 1.0*instr/diff.total_microseconds());
    printf("ms:   %lld\n", diff.total_milliseconds());
}

std::string CPU::disas(WORD i)
{
    BYTE op =  i          & 0x1F;
    WORD a  =  i >> 0x0A;
    WORD b  = (i >> 0x05) & 0x1F;
    
    std::string ops = "", as = "", bs = "";
    
    switch(a)
    {
        case 0x00:
            as = "A";
            break;
        case 0x01:
            as = "B";
            break;
        case 0x02:
            as = "C";
            break;
        case 0x03:
            as = "X";
            break;
        case 0x04:
            as = "Y";
            break;
        case 0x05:
            as = "Z";
            break;
        case 0x06:
            as = "I";
            break;
        case 0x07:
            as = "J";
            break;
        case 0x08:
            as = "[A]";
            break;
        case 0x09:
            as = "[B]";
            break;
        case 0x0A:
            as = "[C]";
            break;
        case 0x0B:
            as = "[X]";
            break;
        case 0x0C:
            as = "[Y]";
            break;
        case 0x0D:
            as = "[Z]";
            break;
        case 0x0E:
            as = "[I]";
            break;
        case 0x0F:
            as = "[J]";
            break;
        case 0x10:
            as = "[A + next]";
            break;
        case 0x11:
            as = "[B + next]";
            break;
        case 0x12:
            as = "[C + next]";
            break;
        case 0x13:
            as = "[X + next]";
            break;
        case 0x14:
            as = "[Y + next]";
            break;
        case 0x15:
            as = "[Z + next]";
            break;
        case 0x16:
            as = "[I + next]";
            break;
        case 0x17:
            as = "[J + next]";
            break;
        case 0x18:
            if(op == 0 && (b == 0x09 || b == 0x10))
                as = "PUSH";
            else
                as = "POP";
            break;
        case 0x19:
            as = "PEEK";
            break;
        case 0x1A:
            as = "PICK";
            break;
        case 0x1B:
            as = "SP";
            break;
        case 0x1C:
            as = "PC";
            break;
        case 0x1D:
            as = "EX";
            break;
        case 0x1E:
            as = "[next]";
            break;
        case 0x1F:
            as = "next";
            break;
        default:
            as = "a-0x21";
            break;
    }
    switch(b)
    {
        case 0x00:
            bs = "A";
            break;
        case 0x01:
            bs = "B";
            break;
        case 0x02:
            bs = "C";
            break;
        case 0x03:
            bs = "X";
            break;
        case 0x04:
            bs = "Y";
            break;
        case 0x05:
            bs = "Z";
            break;
        case 0x06:
            bs = "I";
            break;
        case 0x07:
            bs = "J";
            break;
        case 0x08:
            bs = "[A]";
            break;
        case 0x09:
            bs = "[B]";
            break;
        case 0x0A:
            bs = "[C]";
            break;
        case 0x0B:
            bs = "[X]";
            break;
        case 0x0C:
            bs = "[Y]";
            break;
        case 0x0D:
            bs = "[Z]";
            break;
        case 0x0E:
            bs = "[I]";
            break;
        case 0x0F:
            bs = "[J]";
            break;
        case 0x10:
            bs = "[A + next]";
            break;
        case 0x11:
            bs = "[B + next]";
            break;
        case 0x12:
            bs = "[C + next]";
            break;
        case 0x13:
            bs = "[X + next]";
            break;
        case 0x14:
            bs = "[Y + next]";
            break;
        case 0x15:
            bs = "[Z + next]";
            break;
        case 0x16:
            bs = "[I + next]";
            break;
        case 0x17:
            bs = "[J + next]";
            break;
        case 0x18:
            bs = "PUSH";
            break;
        case 0x19:
            bs = "PEEK";
            break;
        case 0x1A:
            bs = "PICK";
            break;
        case 0x1B:
            bs = "SP";
            break;
        case 0x1C:
            bs = "PC";
            break;
        case 0x1D:
            bs = "EX";
            break;
        case 0x1E:
            bs = "[next]";
            break;
        case 0x1F:
            bs = "next";
            break;
        default:
            bs = "a-0x21";
            break;
    }
    
    switch(op)
    {
    case 0x00: // special
    {
        switch(b)
        {
        case 0x01: // JSR a
            ops = "JSR";
            break;
        case 0x08: // INT a
            ops = "INT";
            break;
        case 0x09: // IAG a
            ops = "IAG";
            break;
        case 0x0A: // IAS a
            ops = "IAS";
            break;
        case 0x0B: // RFI a
            ops = "RFI";
            break;
        case 0x0C: // IAQ a
            ops = "IAQ";
            break;
        case 0x10: // HWN a
            ops = "HWN";
            break;
        case 0x11: // HWQ a
            ops = "HWQ";
            break;
        case 0x12: // HWI a
            ops = "HWI";
            break;
        default:
            return "invalid";
            break;
        }
        return ops + " " + as;
        break;
    }
    case 0x01: // SET b, a
        ops = "SET";
        break;
    case 0x02: // ADD b, a
        ops = "ADD";
        break;
    case 0x03: // SUB b, a
        ops = "SUB";
        break;
    case 0x04: // MUL b, a
        ops = "MUL";
        break;
    case 0x05: // MLI b, a
        ops = "MLI";
        break;
    case 0x06: // DIV b, a
        ops = "DIV";
        break;
    case 0x07: // DVI b, a
        ops = "DVI";
        break;
    case 0x08: // MOD b, a
        ops = "MOD";
        break;
    case 0x09: // MDI b, a
        ops = "MDI";
        break;
    case 0x0A: // AND b, a
        ops = "AND";
        break;
    case 0x0B: // BOR b, a
        ops = "BOR";
        break;
    case 0x0C: // XOR b, a
        ops = "XOR";
        break;
    case 0x0D: // SHR b, a
        ops = "SHR";
        break;
    case 0x0E: // ASR b, a
        ops = "ASR";
        break;
    case 0x0F: // SHL b, a
        ops = "SHL";
        break;
    case 0x10: // IFB b, a
        ops = "IFB";
        break;
    case 0x11: // IFC b, a
        ops = "IFC";
        break;
    case 0x12: // IFE b, a
        ops = "IFE";
        break;
    case 0x13: // IFN b, a
        ops = "IFN";
        break;
    case 0x14: // IFG b, a
        ops = "IFG";
        break;
    case 0x15: // IFA b, a
        ops = "IFA";
        break;
    case 0x16: // IFL b, a
        ops = "IFL";
        break;
    case 0x17: // IFU b, a
        ops = "IFU";
        break;
    case 0x1A: // ADX b, a
        ops = "ADX";
        break;
    case 0x1B: // SBX b, a 
        ops = "SBX";
        break;
    case 0x1E: // STI b, a
        ops = "STI";
        break;
    case 0x1F: // STD b, a
        ops = "STD";
        break;
    default:
        return "invalid";
        break;
    }
    return ops + " " + bs + " " + as;
}

void CPU::addDevice(Hardware *dev)
{
    mDevices.push_back(dev);
}

int CPU::getNumberDevices()
{
    return mDevices.size();
}

Hardware *CPU::getDevice(int i)
{
    return mDevices.at(i);
}

void CPU::setMaterial()
{
    SpaceShipPart::setMaterial("Base/Black");
}