#ifndef _SPACESHIPPART_HPP_
#define _SPACESHIPPART_HPP_

#include "Entity.hpp"

#include <vector>

class SpaceShipPart : public Entity
{
public:
    enum PART_TYPE
    {
        PART_FLOOR,
        PART_WALL,
        PART_FLOORMOUNT,
        PART_WALLMOUNT,
        PART_CEILMOUNT
    };

    class SpaceShipPartInfo
    {
    public:
        SpaceShipPartInfo(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion rot, bool placable);
        PART_TYPE mPartType;
        Ogre::Vector3 mPos;
        Ogre::Quaternion mRot;
        bool mPlacable;
    };

    SpaceShipPart(PART_TYPE partType, bool castShadows, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine);
    SpaceShipPart(SpaceShipPart *old, Ogre::String name, Ogre::String type = "");
    virtual ~SpaceShipPart();
    
    virtual bool update(float elapsedTime) = 0;

    PART_TYPE getPartType() {return mPartType;}
    size_t getNumberNeighbors() {return mNeighbor.size();}
    SpaceShipPart *getNeighbor(size_t i) {return mNeighbor.at(i).first;}
    SpaceShipPartInfo *getNeighborInfo(size_t i) {return mNeighbor.at(i).second;}
    void setNeighbor(SpaceShipPart *p, size_t i) {mNeighbor.at(i).first = p;}

    void setMaterial(Ogre::String name);

protected:
    Ogre::Entity *mEntity;
    std::vector<std::pair<SpaceShipPart *, SpaceShipPartInfo *> > mNeighbor;
    PART_TYPE mPartType;
};

#endif
