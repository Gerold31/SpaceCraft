#ifndef _MEMORY_HPP_
#define _MEMORY_HPP_

#include "Entity.hpp"

#include <string>

typedef unsigned short WORD;
typedef unsigned char  BYTE;

class Memory : public Entity
{
public:
    Memory(std::string fileName, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    ~Memory();
    
    bool update(float elapsedTime){return true;};

    WORD *getMemory() {return mMem;}

private:
    WORD mMem[1<<16];
};

#endif