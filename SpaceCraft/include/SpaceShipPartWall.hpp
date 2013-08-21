#ifndef _SPACESHIPPARTWALL_HPP_
#define _SPACESHIPPARTWALL_HPP_

#include "SpaceShipPart.hpp"

#define  WALL_SIZE_X (0.2)
#define  WALL_SIZE_Y (2.2)
#define  WALL_SIZE_Z (1.2)

class SpaceShipPartWall : public SpaceShipPart
{
public:
    SpaceShipPartWall(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine, Ogre::String type = mType);
    SpaceShipPartWall(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine, Ogre::String type = mType);
    SpaceShipPartWall(SpaceShipPart *old, Ogre::String name, Ogre::String type = mType);

    bool update(float elapsedTime);
    virtual void setMaterial();

    static std::string getType(){return mType;}

private:
    static std::string mType;

    static SpaceShipPartInfo mPartInfo[14];

    void commonConstructor();
};

#endif
