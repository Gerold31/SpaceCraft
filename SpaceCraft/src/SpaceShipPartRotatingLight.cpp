#include "SpaceShipPartRotatingLight.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLight.h"

SpaceShipPart::SpaceShipPartInfo SpaceShipPartRotatingLight::mPartInfo[] = {SpaceShipPartInfo(PART_FLOOR, Ogre::Vector3(0, 0.05, 0), Ogre::Quaternion(1, 0, 0, 0), true)};

std::string SpaceShipPartRotatingLight::mType = "SC_SpaceShipPartRotatingLight";

SpaceShipPartRotatingLight::SpaceShipPartRotatingLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_CEILMOUNT, true, pos, ori, parent, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "RotatingLight.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    commonConstructor();
}

SpaceShipPartRotatingLight::SpaceShipPartRotatingLight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, ENGINE *engine)
    : SpaceShipPart(PART_CEILMOUNT, true, pos, ori, parent, staticGeometry, name, mType, engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "RotatingLight.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
	//setupInstancedMaterialToEntity(mEntity);
    staticGeometry->addEntity(mEntity, pos, ori);

    commonConstructor();
}

void SpaceShipPartRotatingLight::commonConstructor()
{
    for(int i=0; i<sizeof(mPartInfo)/sizeof(SpaceShipPartInfo); i++)
    {
        mNeighbor.push_back(std::pair<SpaceShipPart *, SpaceShipPartInfo *>(NULL, &mPartInfo[i]));
    }

    mLight = mEngine->getSceneMgr()->createLight(mName + "Light");
    mLight->setType(Ogre::Light::LT_SPOTLIGHT);
    mLight->setPosition(0,-0.025,0);
    mLight->setDiffuseColour(1, 0.2, 0);
    mLight->setSpecularColour(1, 0.2, 0);
    mLight->setDirection(1, 0, 0);
    mLight->setSpotlightRange(Ogre::Degree(45), Ogre::Degree(90));
    mLight->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mLight);
}

bool SpaceShipPartRotatingLight::update(float elapsedTime)
{
    mLight->setDirection(Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI*5*elapsedTime), Ogre::Vector3::UNIT_Y) * mLight->getDirection());
    return true;
}

void SpaceShipPartRotatingLight::setMaterial()
{
    SpaceShipPart::setMaterial("Base/Red");
}
