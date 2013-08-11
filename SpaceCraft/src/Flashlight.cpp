#include "FlashLight.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

#define LIGHT_COLOR Ogre::ColourValue(0.9, 0.9, 0.9)
#define LIGHT_RANGE (100.0)

Flashlight::Flashlight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, "SC_Flashlight", engine)
{
    mEntity = engine->getSceneMgr()->createEntity("Flashlight.mesh");
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);

    mLight = engine->getSceneMgr()->createLight(name + "Light");
    mLight->setType(Ogre::Light::LT_SPOTLIGHT);
    mLight->setPosition(0,0,-0.1);
    mLight->setDiffuseColour(0,0,0);
    mLight->setSpecularColour(0,0,0);
    mLight->setDirection(0, 0, -1);
    mLight->setSpotlightRange(Ogre::Degree(12), Ogre::Degree(45));
    mLight->setAttenuation(LIGHT_RANGE, 1.0, 4.5/LIGHT_RANGE, 75.0/(LIGHT_RANGE*LIGHT_RANGE));
    mLight->setCastShadows(true);
    mLight->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mLight);
}

Flashlight::~Flashlight()
{
    mEngine->getSceneMgr()->destroyLight(mLight);
}

bool Flashlight::update(float elapsedTime)
{
    return true;
}

void Flashlight::toggle()
{
    if(mLight->getDiffuseColour() == LIGHT_COLOR)
        toggle(false);
    else
        toggle(true);
}

void Flashlight::toggle(bool toggle)
{
    if(toggle)
    {    
        mLight->setDiffuseColour(LIGHT_COLOR);
        mLight->setSpecularColour(LIGHT_COLOR);
    }else
    {
        mLight->setDiffuseColour(0,0,0);
        mLight->setSpecularColour(0,0,0);
    }
}