#ifndef _SPACESHIPPARTLIGHT_HPP_
#define _SPACESHIPPARTLIGHT_HPP_

#include "SpaceShipPart.hpp"

namespace Ogre
{
    class Light;
};

class SpaceShipPartLight : public SpaceShipPart
{
public:
    SpaceShipPartLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);

    void setBrightness(int b);

private:
    static SpaceShipPartInfo mPartInfo[1];

    Ogre::Light *mLight;
};

#endif
