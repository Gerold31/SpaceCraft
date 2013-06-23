#ifndef _KINO_HPP_
#define _KINO_HPP_

#include "Entity.hpp"

namespace Ogre
{
    class Entity;
    class Camera;
};
#include "OGRE/OgreString.h"

#include "OIS/OIS.h"

class Kino : public Entity, public OIS::KeyListener, public OIS::MouseListener
{
public:
    Kino(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);

    bool update(float elapsedTime);
    
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
 
    bool mouseMoved(const OIS::MouseEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    void stop() {mTranslation = Ogre::Vector3::ZERO;}

    Ogre::Camera *getCamera() {return mCamera;}

private:
    Ogre::Entity *mEntity;

    Ogre::SceneNode *mCameraYawNode;
    Ogre::SceneNode *mCameraPitchNode;
    Ogre::SceneNode *mCameraRollNode;
    Ogre::Camera *mCamera;

    Ogre::Vector3 mTranslation;
};

#endif