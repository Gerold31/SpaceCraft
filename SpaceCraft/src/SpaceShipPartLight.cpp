#include "SpaceShipPartLight.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLight.h"

SpaceShipPart::SpaceShipPartInfo SpaceShipPartLight::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, 0.05, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

SpaceShipPartLight::SpaceShipPartLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_CEILMOUNT, true, pos, ori, parent, name, "SC_SpaceShipPartLight", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "Light.mesh");
    mNode->attachObject(mEntity);
    
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }

    mLight = engine->getSceneMgr()->createLight(name + "Light");
    mLight->setType(Ogre::Light::LT_POINT);
    mLight->setPosition(0,-0.025,0);
    mLight->setDiffuseColour(0.9, 0.8, 0.8);
    mLight->setSpecularColour(0.9, 0.8, 0.8);
    mNode->attachObject(mLight);
}

bool SpaceShipPartLight::update(float elapsedTime)
{
    return true;
}

void SpaceShipPartLight::setBrightness(int b)
{
    mLight->setDiffuseColour(0.9*b/256, 0.8*b/256, 0.8*b/256);
    mLight->setSpecularColour(0.9*b/256, 0.8*b/256, 0.8*b/256);
}