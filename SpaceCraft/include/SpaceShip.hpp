#ifndef _SPACESHIP_HPP_
#define _SPACESHIP_HPP_

#include "GravityObject.hpp"

namespace Ogre
{
    class StaticGeometry;
}

class SpaceShipPart;

class SpaceShip : public GravityObject
{
public:
    SpaceShip(double mass, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    ~SpaceShip();
    
    void addPart(SpaceShipPart *newPart);
    void removePart(SpaceShipPart *part);
    size_t getNumberOfParts() {return mParts.size();}
    SpaceShipPart *getPart(size_t i) {return mParts.at(i);}
    int getNextPartID() {return mNextPartID;}
    
    void save(std::string fileName);
    void load(std::string fileName);

private:
    Ogre::StaticGeometry *mStaticGeometry;

    std::vector<SpaceShipPart *>mParts;
    int mNextPartID;
};

#endif