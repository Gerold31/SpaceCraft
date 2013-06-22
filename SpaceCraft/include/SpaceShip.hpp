#ifndef _SPACESHIP_HPP_
#define _SPACESHIP_HPP_

#include "GravityObject.hpp"

class SpaceShipPart;

class SpaceShip : public GravityObject
{
public:
    SpaceShip(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    
    virtual bool update(float elapsedTime){return true;}

    void addPart(SpaceShipPart *newPart); 
    int getNumberOfParts() {return mNumberOfParts;}

private:
    SpaceShipPart *mRootPart;
    int mNumberOfParts;
};

#endif