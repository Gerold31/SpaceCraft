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
    MessageDisplaySetImage(Ogre::Image &img) : Message(getID(), false, true, false, true, false), mImg(img) {LOG_IN_MSG; LOG_OUT_MSG;}
    static void *CreateMessage(std::istream &stream) 
    {
        LOG_IN_MSG;
        int len;
        stream >> len;
        char *data = new char[len+1];
        stream.read(data, len+1);
        Ogre::DataStreamPtr d(new Ogre::MemoryDataStream(data+1, len, false));
        Ogre::Image img;
        img.load(d, "png");
        delete[] data;
        void *m = new MessageDisplaySetImage(img);
        LOG_OUT_MSG;
        return m; 
    }
    static int getID() {if(mID == -1) mID = calcID("MessageDisplaySetImage"); return mID;}
    Ogre::Image mImg;
private:
    void _serialize(std::ostream &stream) 
    {
        LOG_IN_MSG;
        Ogre::String data = mImg.encode("png")->getAsString(); 
        stream << data.length() << std::endl;
        stream.write(data.data(), data.length());
        stream << std::endl;
        LOG_OUT_MSG;
    }
    static int mID;
};

};

#endif
