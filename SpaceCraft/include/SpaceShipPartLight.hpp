#ifndef _SPACESHIPPARTLIGHT_HPP_
#define _SPACESHIPPARTLIGHT_HPP_

#include "SpaceShipPart.hpp"

namespace Ogre
{
    class Light;
};

class CPULightControl;

class SpaceShipPartLight : public SpaceShipPart
{
public:
    SpaceShipPartLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    SpaceShipPartLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);
    ~SpaceShipPartLight();

    bool update(float elapsedTime);

    void setBrightness(int b);
    
    void connect(CPULightControl *ctrl) {mControl = ctrl;}
    bool isConnected() {return mControl;}

    int onHit(int damage);

    static std::string getType(){return mType;}

private:
    static std::string mType;

    static SpaceShipPartInfo mPartInfo[1];

    Ogre::Light *mLight;

    CPULightControl *mControl;

    int mHealth;

    void commonConstructor();
};

#endif
