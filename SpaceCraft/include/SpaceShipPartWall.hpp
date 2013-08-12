#ifndef _SPACESHIPPARTWALL_HPP_
#define _SPACESHIPPARTWALL_HPP_

#include "SpaceShipPart.hpp"

#define  WALL_SIZE_X (0.2)
#define  WALL_SIZE_Y (2.2)
#define  WALL_SIZE_Z (1.2)

class SpaceShipPartWall : public SpaceShipPart
{
public:
    SpaceShipPartWall(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine, Ogre::String type = "SC_SpaceShipPartWall");
    SpaceShipPartWall(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine, Ogre::String type = "SC_SpaceShipPartWall");
    SpaceShipPartWall(SpaceShipPart *old, Ogre::String name, Ogre::String type = "SC_SpaceShipPartWall");

    bool update(float elapsedTime);

private:
    static SpaceShipPartInfo mPartInfo[10];

    void commonConstructor();
};

#endif
