#include "ComponentCPU.hpp"

#include "TypeInfo.hpp"
#include "SystemLog.hpp"
#include "ComponentHardware.hpp"
#include "ComponentMemory.hpp"
#include "MessageCPU.hpp"

using namespace SpaceCraft;

TypeInfo *ComponentCPU::mType = new TypeInfo("ComponentCPU", &ComponentCPU::createInstance);

ComponentCPU::ComponentCPU(Object *object, ParamMap &params) :
    Component(object, params, mType),
    mFrequency(1000000.0)
{
    LOG_IN("component");
    mRunning = false;
    mInterrupQueuing = false;
    mInstructionsSinceInterrupt = 0;
    mInterrupts = 0;
    reset();
    LOG_OUT("component");
}

ComponentCPU::~ComponentCPU()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void *ComponentCPU::createInstance(Object *object, ParamMap &params)
{
    LOG_IN("component");
    LOG_OUT("component");
    return new ComponentCPU(object, params);
}

void ComponentCPU::init()
{
    LOG_IN("component");
    LOG_OUT("component");
}

void ComponentCPU::update(float elapsedTime)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}

void ComponentCPU::receiveMessage(Message *message)
{
    LOG_IN_FRAME;
    LOG_OUT_FRAME;
}


void ComponentCPU::interrupt(WORD msg)
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

void ComponentCPU::setMemory(ComponentMemory *mem)
{
    mRam = mem->getMemory();
}

void ComponentCPU::start()
{
    mRunning = true;
    mThread = boost::thread(boost::bind(&ComponentCPU::run, this));
}

void ComponentCPU::stop()
{
    mRunning = false;
    mThread.join();
}

void ComponentCPU::reset()
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

void ComponentCPU::run()
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
                        ComponentHardware *dev = mDevices.at(a);
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
                        ComponentHardware *dev = mDevices.at(a);
                        if(dev)
                        {
                            MessageInterrupt m(0);
                            m.sendTo(dev);
                        }
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
