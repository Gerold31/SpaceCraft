#include "SpaceShip.hpp"

#include "ENGINE.hpp"

#include "SpaceShipPart.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"

SpaceShip::SpaceShip(double mass, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :GravityObject(mass, velocity, pos, ori, parent, name, "SC_SpaceShip", engine)
{
    mParts.push_back(new SpaceShipPart(SpaceShipPart::PART_FLOOR, pos, ori, mNode, name + "Part0", engine));
    mNextPartID = 1;
}

void SpaceShip::addPart(SpaceShipPart *newPart)
{
    mParts.push_back(newPart);
    mNextPartID++;
}