#include "SpaceShipPart.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoFloor[]     = {SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 2),
                                                                        SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0, 1  ), Ogre::Quaternion(1, 0, 0, 0), 3),
                                                                        SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3(-1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 0),
                                                                        SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0,-1  ), Ogre::Quaternion(1, 0, 0, 0), 1),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3( 0.5, 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 0),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3( 0  , 0, 0.5), Ogre::Quaternion(sqrt(0.5), 0, sqrt(0.5), 0), 0),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3(-0.5, 0, 0  ), Ogre::Quaternion(1, 0, 1, 0), 0),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3( 0  , 0,-0.5), Ogre::Quaternion(sqrt(0.5), 0, -sqrt(0.5), 0), 0),
                                                                        SpaceShipPartInfo(PART_INTERIOR,  Ogre::Vector3( 0  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 0),
                                                                        SpaceShipPartInfo(PART_INNERWALL, Ogre::Vector3( 0  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 0) };
SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoCeil[]      = {SpaceShipPartInfo(PART_CEIL,      Ogre::Vector3( 1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 2),
                                                                        SpaceShipPartInfo(PART_CEIL,      Ogre::Vector3( 0  , 0, 1  ), Ogre::Quaternion(1, 0, 0, 0), 3),
                                                                        SpaceShipPartInfo(PART_CEIL,      Ogre::Vector3(-1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 0),
                                                                        SpaceShipPartInfo(PART_CEIL,      Ogre::Vector3( 0  , 0,-1  ), Ogre::Quaternion(1, 0, 0, 0), 1),
                                                                        SpaceShipPartInfo(PART_INTERIOR,  Ogre::Vector3( 0  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 0) };
SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoOuterWall[] = {SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0,-0.5), Ogre::Quaternion(1, 0, 0, 0), -1),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3( 1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 2),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3(-1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 1),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3( 0  , 2, 0  ), Ogre::Quaternion(1, 0, 0, 0), 4),
                                                                        SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3( 0  ,-2, 0  ), Ogre::Quaternion(1, 0, 0, 0), 3),
                                                                        SpaceShipPartInfo(PART_INTERIOR,  Ogre::Vector3( 0  , 1, 0  ), Ogre::Quaternion(1, 0, 0, 0), 0) };
SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoInnerWall[] = {SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 9),
                                                                        SpaceShipPartInfo(PART_INNERWALL, Ogre::Vector3( 1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 1),
                                                                        SpaceShipPartInfo(PART_INNERWALL, Ogre::Vector3(-1  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 1),
                                                                        SpaceShipPartInfo(PART_INNERWALL, Ogre::Vector3( 0  , 2, 0  ), Ogre::Quaternion(1, 0, 0, 0), 4),
                                                                        SpaceShipPartInfo(PART_INNERWALL, Ogre::Vector3( 0  ,-2, 0  ), Ogre::Quaternion(1, 0, 0, 0), 3),
                                                                        SpaceShipPartInfo(PART_INTERIOR,  Ogre::Vector3( 0  , 1,-1  ), Ogre::Quaternion(1, 0, 0, 0), 0),
                                                                        SpaceShipPartInfo(PART_INTERIOR,  Ogre::Vector3( 0  , 1, 1  ), Ogre::Quaternion(1, 0, 0, 0), 0) };
SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoInterior[]  = {SpaceShipPartInfo(PART_FLOOR,     Ogre::Vector3( 0  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 8) };
SpaceShipPart::SpaceShipPartInfo SpaceShipPart::mPartInfoExterior[]  = {SpaceShipPartInfo(PART_OUTERWALL, Ogre::Vector3( 0  , 0, 0  ), Ogre::Quaternion(1, 0, 0, 0), 9) };

SpaceShipPart::SpaceShipPartInfo::SpaceShipPartInfo(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion rot, int otherID)
{
    mPartType = partType;
    mPos = pos;
    mRot = rot;
    mOtherID = otherID;
}

SpaceShipPart::SpaceShipPart(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Entity(pos, ori, parent, name, "SC_SpaceShipPart", engine)
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
    case PART_CEIL:
        mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Ceil.mesh");
        mNode->attachObject(mEntity);
        for(int i=0; i<sizeof(mPartInfoCeil)/sizeof(SpaceShipPartInfo); i++)
        {
            mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfoCeil[i]));
        }
        break;
    case PART_OUTERWALL:
        mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Wall.mesh");
        mNode->attachObject(mEntity);
        for(int i=0; i<sizeof(mPartInfoOuterWall)/sizeof(SpaceShipPartInfo); i++)
        {
            mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfoOuterWall[i]));
        }
        break;
    case PART_INNERWALL:
        mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Wall.mesh");
        mNode->attachObject(mEntity);
        for(int i=0; i<sizeof(mPartInfoInnerWall)/sizeof(SpaceShipPartInfo); i++)
        {
            mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfoInnerWall[i]));
        }
        break;
    case PART_INTERIOR:
        for(int i=0; i<sizeof(mPartInfoInterior)/sizeof(SpaceShipPartInfo); i++)
        {
            mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfoInterior[i]));
        }
        break;
    case PART_EXTERIOR:
        for(int i=0; i<sizeof(mPartInfoExterior)/sizeof(SpaceShipPartInfo); i++)
        {
            mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfoExterior[i]));
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
    case PART_CEIL:
        mEntity = mEngine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Ceil.mesh");
        mNode->attachObject(mEntity);
        break;
    case PART_OUTERWALL:
        mEntity = mEngine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Wall.mesh");
        mNode->attachObject(mEntity);
        break;
    case PART_INNERWALL:
        mEntity = mEngine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Wall.mesh");
        mNode->attachObject(mEntity);
        break;
    case PART_INTERIOR:
    case PART_EXTERIOR:
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
