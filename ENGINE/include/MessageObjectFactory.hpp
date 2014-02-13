#ifndef _MESSAGEOBJECTFACTORY_HPP_
#define _MESSAGEOBJECTFACTORY_HPP_

#include "Message.hpp"

#include <OIS/OIS.h>
#include "OGRE/OgreString.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"

namespace ENGINE
{

class MessageCreateObject : public Message
{
public:
    MessageCreateObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, std::string type = "") : Message(getID(), true, true, false, false, false) {LOG_IN_MSG; mPos = pos; mOri = ori, mParent = parent; mName = name; mType = type; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream);
    static int getID() {if(mID == -1) mID = calcID("MessageCreateObject"); return mID;}

    Ogre::Vector3 mPos;
    Ogre::Quaternion mOri;
    Ogre::SceneNode *mParent;
    Ogre::String mName;
    std::string mType;
private:
    void _serialize(std::ostream &stream);

    static int mID;
};

};

#endif
