#include "SpaceShipPart.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"

#define FLOOR_SIZE_X (1.0)
#define FLOOR_SIZE_Y (0.2)
#define FLOOR_SIZE_Z (1.0)
#define  WALL_SIZE_X (0.2)
#define  WALL_SIZE_Y (2.0)
#define  WALL_SIZE_Z (1.0)


SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoFloor[]    = {SpaceShipPartInfo(PART_FLOORMOUNT,Ogre::Vector3(0, FLOOR_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), false),
                                                                       SpaceShipPartInfo(PART_CEILMOUNT, Ogre::Vector3(0,-FLOOR_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), false),

                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X, 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0, FLOOR_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X, 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0,-FLOOR_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), true),

                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0)                 , true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0, -sqrt(0.5), 0), true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(0, 0, 1, 0)                 , true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0,  sqrt(0.5), 0), true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0)                 , true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0, -sqrt(0.5), 0), true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(0, 0, 1, 0)                 , true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(sqrt(0.5), 0,  sqrt(0.5), 0), true)};

SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoWall[]     = {SpaceShipPartInfo(PART_WALLMOUNT, Ogre::Vector3(-WALL_SIZE_X/2, 0, 0), Ogre::Quaternion(1, 0, 0, 0), false),
                                                                       SpaceShipPartInfo(PART_WALLMOUNT, Ogre::Vector3( WALL_SIZE_X/2, 0, 0), Ogre::Quaternion(1, 0, 0, 0), false),

                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, 0, WALL_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), false),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, 0,-WALL_SIZE_Z), Ogre::Quaternion(1, 0, 0, 0), false),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0, WALL_SIZE_Y, 0), Ogre::Quaternion(1, 0, 0, 0),  true),
                                                                       SpaceShipPartInfo(PART_WALL,      Ogre::Vector3(0,-WALL_SIZE_Y, 0), Ogre::Quaternion(1, 0, 0, 0),  true),

                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X/2, WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true),
                                                                       SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( FLOOR_SIZE_X/2,-WALL_SIZE_Y/2, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

SpaceShipPart::SpaceShipPartInfo::SpaceShipPartInfo(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion rot, bool placable)
{
    mPartType = partType;
    mPos = pos;
    mRot = rot;
    mPlacable = placable;
}

SpaceShipPart::SpaceShipPart(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type,ENGINE *engine)
    :Entity(pos, ori, parent, name, type, engine)
{
    mPartType = partType;
    mEntity = NULL;
    switch(mPartType)
    {
    case PART_FLOOR:
        mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Floor.mesh");
        mNode->attachObject(mEntity);
        for(int i=0; i<sizeof(mPartInfoFloor)/sizeof(SpaceShipPartInfo); i++)
        {
            mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfoFloor[i]));
        }
        break;
    case PART_WALL:
        mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Wall.mesh");
        mNode->attachObject(mEntity);
        for(int i=0; i<sizeof(mPartInfoWall)/sizeof(SpaceShipPartInfo); i++)
        {
            mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfoWall[i]));
        }
        break;
    default:
        return;
    }
}

SpaceShipPart::SpaceShipPart(SpaceShipPart *old, Ogre::String name)
    :Entity(old->getParentSceneNode()->getPosition(), old->getParentSceneNode()->getOrientation(), old->getParentSceneNode()->getParentSceneNode(), name, "SC_SpaceShipPart", old->mEngine)
{
    mPartType = old->mPartType;
    mNeighbor = old->mNeighbor;
    switch(mPartType)
    {
    case PART_FLOOR:
        mEntity = mEngine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Floor.mesh");
        mNode->attachObject(mEntity);
        break;
    case PART_WALL:
        mEntity = mEngine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Wall.mesh");
        mNode->attachObject(mEntity);
        break;
    default:
        return;
    }
}

SpaceShipPart::~SpaceShipPart()
{
    if(mEntity)
        mEngine->getSceneMgr()->destroyEntity(mEntity);
    mEntity = NULL;
}

void SpaceShipPart::setMaterial(Ogre::String name)
{
    if(mEntity)
        mEntity->setMaterialName(name);
}
