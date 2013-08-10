#ifndef _CPULIGHTCONTROL_HPP_
#define _CPULIGHTCONTROL_HPP_

#include "Hardware.hpp"

class SpaceShipPartLight;

#include <vector>

class CPULightControl : public Hardware
{
public:
    CPULightControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void interrupt();

    void addLight(SpaceShipPartLight *light);

private:
    std::vector<SpaceShipPartLight *>mLights;

    static SpaceShipPartInfo mPartInfo[1];
};

#endif