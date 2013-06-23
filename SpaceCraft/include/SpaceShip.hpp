#ifndef _SPACESHIP_HPP_
#define _SPACESHIP_HPP_

#include "GravityObject.hpp"

class SpaceShipPart;

class SpaceShip : public GravityObject
{
public:
    SpaceShip(double mass, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    
    void addPart(SpaceShipPart *newPart); 
    int getNumberOfParts() {return mNumberOfParts;}

private:
    SpaceShipPart *mRootPart;
    int mNumberOfParts;
};

#endif