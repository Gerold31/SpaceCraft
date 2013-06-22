#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

namespace Ogre
{
    class SceneManager;
    class SceneNode;
    class RenderQueue;
    class Renderable;
};

class ENGINE;
class Map;

#include "OGRE/OgreString.h"
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreQuaternion.h"
#include "OGRE/OgreAxisAlignedBox.h"

#include "OGRE/OgreManualObject.h"

class Entity : public Ogre::ManualObject
{
public:
    Entity(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine);

    virtual void init(){}
    virtual bool update(float elapsedTime){return true;}
    Ogre::String getType() {return mType;}
    Ogre::SceneNode *getSceneNode() {return mNode;}

protected: 
    Ogre::SceneNode *mNode;
    const Ogre::String mType;
    Ogre::AxisAlignedBox mBoundingBox;
    ENGINE *mEngine;
    Map *mMap;
};

#endif