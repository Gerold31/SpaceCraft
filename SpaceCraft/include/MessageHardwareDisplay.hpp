#ifndef _MESSAGEHARDWAREDISPLAY_HPP_
#define _MESSAGEHARDWAREDISPLAY_HPP_

#include "Defines.hpp"
#include "SystemLog.hpp"
#include "SpaceCraftDefines.hpp"
#include "Message.hpp"

#include "OGRE/OgreImage.h"
#include "OGRE/OgreDataStream.h"

#include <iostream>

using namespace ENGINE;

namespace SpaceCraft
{

class MessageDisplaySetImage : public Message
{
public:
    MessageDisplaySetImage(Ogre::Image &img) : Message(getID(), false, true, false, true, false), mImg(img) {}
    static void *CreateMessage(std::istream &stream) {int len; stream >> len; char *data = new char[len+1]; stream.read(data, len+1); Ogre::DataStreamPtr d(new Ogre::MemoryDataStream(data+1, len, false)); delete[] data; Ogre::Image img; img.load(d, "png"); void *m = new MessageDisplaySetImage(img); return m; }
    static int getID() {if(mID == -1) mID = calcID("MessageDisplaySetImage"); return mID;}
    Ogre::Image mImg;
private:
    void _serialize(std::ostream &stream) { Ogre::String data = mImg.encode("png")->getAsString(); stream << data.length() << std::endl; stream.write(data.c_str(), data.length()); stream << std::endl;}
    static int mID;
};

};

#endif
