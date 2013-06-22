#include "SpaceShipPart.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPart(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Entity(pos, ori, parent, name, "SC_SpaceShipPart", engine)
{
    mPartType = partType;
    switch(mPartType)
    {
    case PART_FLOOR:
        mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Floor.mesh");
        mNode->attachObject(mEntity);
        mNeighbor.push_back(NULL);
        mNeighbor.push_back(NULL);
        mNeighbor.push_back(NULL);
        mNeighbor.push_back(NULL);
        mNumberNeighbors = 4;
        break;
    case PART_WALL:
        mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "SpaceShip/Part_Wall.mesh");
        mNode->attachObject(mEntity);
        mNeighbor.push_back(NULL);
        mNumberNeighbors = 1;
        break;
    default:
        return;
    }
}