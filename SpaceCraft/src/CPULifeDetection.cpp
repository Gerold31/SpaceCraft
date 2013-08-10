#include "CPULifeDetection.hpp"

#include "ENGINE.hpp"
#include "Map.hpp"
#include "CPU.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

#define ALERT_DISTANCE (10.0)
#define RESOLUTION (1<<8)
#define DIST_DIFF_THRESHOLD (ALERT_DISTANCE/RESOLUTION)

SpaceShipPart::SpaceShipPartInfo CPULifeDetection::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, 0.05, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

CPULifeDetection::CPULifeDetection(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Hardware(0x11F3DE7C, 0x1C6C8B36, 0x0001, PART_CEILMOUNT, pos, ori, parent, name, "CPU_LifeDetection", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPULifeDetection.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
    
    mInterruptMsg = 0;
    mLastDistance = -1;

    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}

bool CPULifeDetection::update(float elapsedTime)
{
    float minDist = -1;
    for(int i=0; i<mEngine->getMap()->getNumberEntities(); i++)
    {
        Entity *ent = mEngine->getMap()->getEntity(i);
        if(ent && ent->getType() == "SC_Human")
        {
            float dist = ent->getSceneNode()->getPosition().distance(mNode->getPosition());
            if(dist < minDist || minDist < 0)
                minDist = dist;
        }
    }
    if(minDist < ALERT_DISTANCE && std::abs(minDist - mLastDistance) > DIST_DIFF_THRESHOLD)
    {
        mLastDistance = minDist;
        if(mInterruptMsg != 0)
            mCPU->interrupt(mInterruptMsg);
    }
    return true;
}

void CPULifeDetection::interrupt()
{
    switch(mCPU->getRegister(0))
    {
	case 0:
		mInterruptMsg = mCPU->getRegister(1);
        break;
    case 1:
        mCPU->setRegister(1, mLastDistance < 0 ? 0 : (RESOLUTION - mLastDistance * RESOLUTION/ALERT_DISTANCE - 1));
        break;
    default:
        break;
    }
}