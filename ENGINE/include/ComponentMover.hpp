#ifndef _COMPONENTMOVER_HPP_
#define _COMPONENTMOVER_HPP_

#include "Component.hpp"

#include "OGRE/OgreVector3.h"

namespace ENGINE
{

class ComponentMover : public Component
{
public:
    ComponentMover(Object *object, ParamMap &params);
    ~ComponentMover();

    static void *createInstance(Object *object, ParamMap &params);
    
    bool init();
    void update(float elapsedTime);
    void receiveMessage(Message *message);

    static TypeInfo *getType() {return mType;}

    Ogre::SceneNode *getYawNode() {return mYawNode;}
    Ogre::SceneNode *getPitchNode() {return mPitchNode;}
    Ogre::SceneNode *getRollNode() {return mRollNode;}

private:
    static TypeInfo *mType;

    double mElapsedTime;

    double mSpeed;
    Ogre::Vector3 mTranslation;
    
    Ogre::SceneNode *mYawNode;
    Ogre::SceneNode *mPitchNode;
    Ogre::SceneNode *mRollNode;

};

};

#endif