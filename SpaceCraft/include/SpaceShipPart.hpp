#ifndef _SPACESHIPPART_HPP_
#define _SPACESHIPPART_HPP_

#include "Entity.hpp"

#include <vector>

class SpaceShipPart : public Entity
{
public:
    enum PART_TYPE
    {
        PART_FLOOR      = 0x1,
        PART_CEIL       = 0x2,
        PART_OUTERWALL  = 0x4,
        PART_INNERWALL  = 0x8,
        PART_INTERIOR   = 0x10,
        PART_EXTERIOR   = 0x20
    };

    class SpaceShipPartInfo
    {
    public:
        SpaceShipPartInfo(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion rot, int otherID);
        PART_TYPE mPartType;
        Ogre::Vector3 mPos;
        Ogre::Quaternion mRot;
        int mOtherID;
    };

    SpaceShipPart(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    SpaceShipPart(SpaceShipPart *old, Ogre::String name);
    ~SpaceShipPart();
    
    virtual bool update(float elapsedTime){return true;}

    PART_TYPE getPartType() {return mPartType;}
    size_t getNumberNeighbors() {return mNeighbor.size();}
    SpaceShipPart *getNeighbor(size_t i) {return mNeighbor.at(i).first;}
    SpaceShipPartInfo *getNeighborInfo(size_t i) {return mNeighbor.at(i).second;}
    void setNeighbor(SpaceShipPart *p, size_t i) {mNeighbor.at(i).first = p;}

    void setMaterial(Ogre::String name);

private:
    Ogre::Entity *mEntity;
    std::vector<std::pair<SpaceShipPart *, SpaceShipPartInfo * >> mNeighbor;
    PART_TYPE mPartType;

    static SpaceShipPartInfo mPartInfoFloor[10], mPartInfoCeil[5], mPartInfoOuterWall[6], mPartInfoInnerWall[7], mPartInfoInterior[1], mPartInfoExterior[1];
};

#endif