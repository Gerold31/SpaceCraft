#include "SpaceShipPartRotatingLight.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLight.h"

SpaceShipPart::SpaceShipPartInfo SpaceShipPartRotatingLight::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, 0.05, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

SpaceShipPartRotatingLight::SpaceShipPartRotatingLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_CEILMOUNT, true, pos, ori, parent, name, "SC_SpaceShipPartRotatingLight", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "RotatingLight.mesh");
    mNode->attachObject(mEntity);
    
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }

    mLight = engine->getSceneMgr()->createLight(name + "Light");
    mLight->setType(Ogre::Light::LT_SPOTLIGHT);
    mLight->setPosition(0,-0.025,0);
    mLight->setDiffuseColour(1, 0.2, 0);
    mLight->setSpecularColour(1, 0.2, 0);
    mLight->setDirection(1, 0, 0);
    mLight->setSpotlightRange(Ogre::Degree(45), Ogre::Degree(90));
    mNode->attachObject(mLight);
}

bool SpaceShipPartRotatingLight::update(float elapsedTime)
{
    mLight->setDirection(Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI*5*elapsedTime), Ogre::Vector3::UNIT_Y) * mLight->getDirection());
    return true;
}
