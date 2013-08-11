#include "CPUDoorControl.hpp"

#include "ENGINE.hpp"

#include "CPU.hpp"
#include "SpaceShipPartDoor.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPartInfo CPUDoorControl::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, -0.5, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

CPUDoorControl::CPUDoorControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Hardware(0xD0012C7l, 0x1C6C8B36, 0x1802, PART_FLOORMOUNT, pos, ori, parent, name, "CPU_DoorControl", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPUDoorControl.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    mInterruptMsg = 0;
    
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}

bool CPUDoorControl::update(float elapsedTime)
{
    for(int i=0; i<mDoors.size(); i++)
    {
        if(mDoors.at(i).first)
        {
            int newState = (mDoors.at(i).first->isLocked() ? 2 : 0) | (mDoors.at(i).first->isOpen() ? 1 : 0);
            if(newState != mDoors.at(i).second)
            {
                mDoors.at(i).second = newState;
                if(mInterruptMsg != 0)
                    mCPU->interrupt(mInterruptMsg);
            }
        }
    }
    return true;
}

void CPUDoorControl::interrupt()
{
    switch(mCPU->getRegister(0))
    {
	case 0:
        mCPU->setRegister(1, mDoors.size());
        break;
	case 1:
    {
        int i = mCPU->getRegister(1);
        if(i < mDoors.size() && mDoors.at(i).first)
        {
            mDoors.at(i).first->open(mCPU->getRegister(2));
            mDoors.at(i).second = (mDoors.at(i).first->isLocked() ? 2 : 0) | (mDoors.at(i).first->isOpen() ? 1 : 0);
        }
        break;
    }
	case 2:
    {
        int i = mCPU->getRegister(1);
        if(i < mDoors.size() && mDoors.at(i).first)
        {
            mDoors.at(i).first->lock(mCPU->getRegister(2));
            mDoors.at(i).second = (mDoors.at(i).first->isLocked() ? 2 : 0) | (mDoors.at(i).first->isOpen() ? 1 : 0);
        }
        break;
    }
    case 3:
        for(int i=0; i<mDoors.size(); i++)
        {
            if(mDoors.at(i).first)
            {
                mDoors.at(i).first->open(mCPU->getRegister(1));
                mDoors.at(i).second = (mDoors.at(i).first->isLocked() ? 2 : 0) | (mDoors.at(i).first->isOpen() ? 1 : 0);
            }
        }
        break;
    case 4:
        for(int i=0; i<mDoors.size(); i++)
        {
            if(mDoors.at(i).first)
            {
                mDoors.at(i).first->lock(mCPU->getRegister(1));
                mDoors.at(i).second = (mDoors.at(i).first->isLocked() ? 2 : 0) | (mDoors.at(i).first->isOpen() ? 1 : 0);
            }
        }
        break;
    case 5:
    {
        int i = mCPU->getRegister(1);
        if(i < mDoors.size() && mDoors.at(i).first)
            mCPU->setRegister(1, mDoors.at(i).second);
        break;
    }
	case 6:
		mInterruptMsg = mCPU->getRegister(1);
        break;
    default:
        break;
    }
}

void CPUDoorControl::addDoor(SpaceShipPartDoor *door)
{
    mDoors.push_back(std::pair<SpaceShipPartDoor *, int>(door, -1));
}