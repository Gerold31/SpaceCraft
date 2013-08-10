#include "CPULightControl.hpp"

#include "ENGINE.hpp"

#include "CPU.hpp"
#include "SpaceShipPartLight.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPartInfo CPULightControl::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, -0.5, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

CPULightControl::CPULightControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Hardware(0x11647C71, 0x1C6C8B36, 0x0001, PART_FLOORMOUNT, pos, ori, parent, name, "CPU_LightControl", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPULightControl.mesh");
    mNode->attachObject(mEntity);
    
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}

bool CPULightControl::update(float elapsedTime)
{
    return true;
}

void CPULightControl::interrupt()
{
    switch(mCPU->getRegister(0))
    {
	case 0:
        mCPU->setRegister(1, mLights.size());
        break;
	case 1:
    {
        int i = mCPU->getRegister(1);
        if(i < mLights.size() && mLights.at(i))
            mLights.at(i)->setBrightness(mCPU->getRegister(2)&0xFF);
        break;
    }
	case 2:
        for(int i=0; i<mLights.size(); i++)
        {
            if(mLights.at(i))
                mLights.at(i)->setBrightness(mCPU->getRegister(1)&0xFF);
        }
        break;
    default:
        break;
    }
}

void CPULightControl::addLight(SpaceShipPartLight *light)
{
    mLights.push_back(light);
}