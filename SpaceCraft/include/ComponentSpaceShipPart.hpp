#ifndef _COMPONENTSPACESHIPPART_HPP_
#define _COMPONENTSPACESHIPPART_HPP_

#include "Component.hpp"

#include "OGRE/OgreVector3.h"
#include "OGRE/OgreQuaternion.h"

using namespace ENGINE;

#define FLOOR_SIZE_X (1.2)
#define FLOOR_SIZE_Y (0.2)
#define FLOOR_SIZE_Z (1.2)
#define  WALL_SIZE_X (0.2)
#define  WALL_SIZE_Y (2.2)
#define  WALL_SIZE_Z (1.2)

namespace SpaceCraft
{

class ComponentSpaceShipPart : public Component
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
        SpaceShipPartInfo(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion rot, bool placable)
        {
            mPartType = partType;
            mPos = pos;
            mRot = rot;
            mPlacable = placable;
        }

        PART_TYPE mPartType;
        Ogre::Vector3 mPos;
        Ogre::Quaternion mRot;
        bool mPlacable;
    };

    ComponentSpaceShipPart(Object *object, ParamMap &params, TypeInfo *type, PART_TYPE partType);
    ~ComponentSpaceShipPart();
    
    bool init();
    void update(float elapsedTime);

    static TypeInfo *getType() {return mType;}

    PART_TYPE getPartType() {return mPartType;}
    size_t getNumberNeighbors() {return mNeighbor.size();}
    ComponentSpaceShipPart *getNeighbor(size_t i) {return mNeighbor.at(i).first;}
    SpaceShipPartInfo *getNeighborInfo(size_t i) {return mNeighbor.at(i).second;}
    void setNeighbor(ComponentSpaceShipPart *p, size_t i) {mNeighbor.at(i).first = p;}

protected:
    std::vector<std::pair<ComponentSpaceShipPart *, SpaceShipPartInfo *> > mNeighbor;

private:
    void _receiveMessage(Message *message);
    static TypeInfo *mType;


    const PART_TYPE mPartType;

};

};

#endif
