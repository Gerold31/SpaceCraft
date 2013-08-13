#ifndef _CPULIFESUPPORT_HPP_
#define _CPULIFESUPPORT_HPP_

#include "Hardware.hpp"

class CPULifeSupport : public Hardware
{
public:
    CPULifeSupport(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    CPULifeSupport(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void interrupt();

    static std::string getType(){return mType;}

private:
    static std::string mType;

    static SpaceShipPartInfo mPartInfo[1];

    void commonConstructor();
};

#endif
