#include "Kino.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"


Kino::Kino(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, "SC_Kino", engine)
{
    mEntity = engine->getSceneMgr()->createEntity("Kino.mesh");
    mNode->attachObject(mEntity);

    mCamera = engine->getSceneMgr()->createCamera(name + "Camera");
    mCamera->lookAt(0,0,-10);
    mCameraYawNode   = mNode->createChildSceneNode();
    mCameraPitchNode = mCameraYawNode->createChildSceneNode();
    mCameraRollNode  = mCameraPitchNode->createChildSceneNode();
    mCameraRollNode->attachObject(mCamera);

	mCamera->setNearClipDistance(1.5f);
	mCamera->setFarClipDistance(3000.0f); 
	mCamera->setAspectRatio(1.0f);
}

bool Kino::update(float elapsedTime)
{
    return true;
}