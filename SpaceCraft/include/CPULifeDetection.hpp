#ifndef _CPULIFEDETECTION_HPP_
#define _CPULIFEDETECTION_HPP_

#include "Hardware.hpp"

class CPULifeDetection : public Hardware
{
public:
    CPULifeDetection(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void interrupt();

private:
    WORD mInterruptMsg;

    float mLastDistance;

    static SpaceShipPartInfo mPartInfo[1];
};

#endif
