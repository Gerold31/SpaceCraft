#ifndef _SPACESHIPPARTFLOOR_HPP_
#define _SPACESHIPPARTFlOOR_HPP_

#include "SpaceShipPart.hpp"

#define FLOOR_SIZE_X (1.2)
#define FLOOR_SIZE_Y (0.2)
#define FLOOR_SIZE_Z (1.2)

class SpaceShipPartFloor : public SpaceShipPart
{
public:
    SpaceShipPartFloor(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    SpaceShipPartFloor(SpaceShipPart *old, Ogre::String name);

    bool update(float elapsedTime);

private:
    static SpaceShipPartInfo mPartInfo[14];
};

#endif
