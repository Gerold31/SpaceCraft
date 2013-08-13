#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

namespace Ogre
{
    class SceneNode;
};

class ENGINE;
class Map;

#include "OGRE/OgreString.h"
#include "OGRE/OgreVector3.h"
#include "OGRE/OgreQuaternion.h"
#include "OGRE/OgreUserObjectBindings.h"
#include "OGRE/OgreAny.h"

#include "OGRE/OgreManualObject.h"

class Entity : public Ogre::ManualObject
{
public:
    Entity(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine);
    virtual ~Entity();

    virtual void init(){}
    virtual bool update(float elapsedTime){return true;}
    Ogre::String getType() {return mType;}

    virtual int onHit(int damage){return 0;}

protected: 
    Ogre::SceneNode *mNode;
    const Ogre::String mType;
    ENGINE *mEngine;
    Map *mMap;
};

#endif
