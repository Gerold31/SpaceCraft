#ifndef _SPACESHIPPARTROTATINGLIGHT_HPP_
#define _SPACESHIPPARTROTATINGLIGHT_HPP_

#include "SpaceShipPart.hpp"

namespace Ogre
{
    class Light;
};

class SpaceShipPartRotatingLight : public SpaceShipPart
{
public:
    SpaceShipPartRotatingLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    SpaceShipPartRotatingLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    void setMaterial();

    static std::string getType(){return mType;}

private:
    static std::string mType;

    static SpaceShipPartInfo mPartInfo[1];

    Ogre::Light *mLight;

    void commonConstructor();
};

#endif
