#ifndef _COMPONENTHARDWARE_HPP_
#define _COMPONENTHARDWARE_HPP_

#include "SpaceCraftDefines.hpp"
#include "Component.hpp"

using namespace ENGINE;

namespace SpaceCraft
{

class ComponentCPU;

class ComponentHardware : public Component
{
public:
    ComponentHardware(Object *object, ParamMap &params, unsigned int id, unsigned int mf, WORD ver);
    ~ComponentHardware();

    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    virtual void interrupt() = 0;

    WORD getIDL() {return  mID        & 0xFFFF;}
    WORD getIDH() {return (mID >> 16) & 0xFFFF;}
    WORD getVER() {return  mVER;}
    WORD getMFL() {return  mMF        & 0xFFFF;}
    WORD getMFH() {return (mMF >> 16) & 0xFFFF;}

protected:
    ComponentCPU *mCPU;
    const unsigned int mID, mMF;
    const WORD mVER;

};

};

#endif
