#ifndef _KINO_HPP_
#define _KINO_HPP_

#include "Entity.hpp"

namespace Ogre
{
    class Entity;
    class Camera;
};
#include "OGRE/OgreString.h"

class Kino : public Entity
{
public:
    Kino(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);

    Ogre::Camera *getCamera() {return mCamera;}
    Ogre::SceneNode *getYawNode() {return mCameraYawNode;}
    Ogre::SceneNode *getPitchNode() {return mCameraPitchNode;}

private:
    Ogre::Entity *mEntity;

    Ogre::SceneNode *mCameraYawNode;
    Ogre::SceneNode *mCameraPitchNode;
    Ogre::SceneNode *mCameraRollNode;
    Ogre::Camera *mCamera;
};

#endif