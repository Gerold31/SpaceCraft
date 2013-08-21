#include "CPULifeSupport.hpp"

#include "ENGINE.hpp"
#include "CPU.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

SpaceShipPart::SpaceShipPartInfo CPULifeSupport::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, -0.5, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

std::string CPULifeSupport::mType = "CPU_LifeSupport";

CPULifeSupport::CPULifeSupport(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Hardware(0x30cf7406, 0x1C6C8B36, 0x1802, PART_FLOORMOUNT, pos, ori, parent, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPULifeSupport.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
    
    commonConstructor();
}

CPULifeSupport::CPULifeSupport(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    :Hardware(0x30cf7406, 0x1C6C8B36, 0x1802, PART_FLOORMOUNT, pos, ori, parent, staticGeometry, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPULifeSupport.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);
    
    commonConstructor();
}

void CPULifeSupport::commonConstructor()
{
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }
}

bool CPULifeSupport::update(float elapsedTime)
{
    return true;
}

void CPULifeSupport::interrupt()
{
    switch(mCPU->getRegister(0))
    {
    default:
        break;
    }
}

void CPULifeSupport::setMaterial()
{
    SpaceShipPart::setMaterial("Base/Black");
}
