#ifndef _CPULIGHTCONTROL_HPP_
#define _CPULIGHTCONTROL_HPP_

#include "Hardware.hpp"

class SpaceShipPartLight;

#include <vector>

class CPULightControl : public Hardware
{
public:
    CPULightControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    CPULightControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    void interrupt();

    void addLight(SpaceShipPartLight *light);
    int getNumberLights() {return mLights.size();}
    SpaceShipPartLight *getLight(int i) {return mLights.at(i);}
    void setLight(int i, SpaceShipPartLight *light) {mLights.at(i) = light;}

    static std::string getType(){return mType;}

private:
    static std::string mType;

    std::vector<SpaceShipPartLight *>mLights;

    static SpaceShipPartInfo mPartInfo[1];

    void commonConstructor();
};

#endif
