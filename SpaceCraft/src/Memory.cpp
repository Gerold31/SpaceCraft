#include "Memory.hpp"

#include <fstream>

std::string Memory::mType = "SC_Memory";

Memory::Memory(std::string fileName, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Entity(pos, ori, parent, name, mType, engine)
{
    std::fstream file(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);
    int memSize = sizeof(mMem)/sizeof(mMem[0]);
    if(file)
    {
        file.seekg (0, file.end);
        int length = file.tellg();
        file.seekg (0, file.beg);

        char *buf = new char [length];

        file.read(buf, length);

        int i;
        for(i=0; i<length/2 && i<memSize; i++)
        {
            mMem[i] = buf[i*2] & 0xFF;
            mMem[i] <<= 8;
            mMem[i] |= buf[i*2+1] & 0xFF;
        }

        if(i < memSize)
        {   
            printf("Warning: File to small\n");
            for(; i<memSize; i++)
            {
                mMem[i] = 0;
                file << '\0' << '\0';
            }
        }

        else if(length/2>memSize)
            printf("Warning: File to big\n");

        delete buf;
        file.close();
    }else
    {
        for(int i=0; i<memSize; i++)
        {
            mMem[i] = 0;
        }
        printf("Error: failed to open file\n");
    }

    for(int i=0; i<memSize/128; i+=8)
    {
        printf("%4x %4x %4x %4x %4x %4x %4x %4x\n", mMem[i], mMem[i+1], mMem[i+2], mMem[i+3], mMem[i+4], mMem[i+5], mMem[i+6], mMem[i+7]); 
    }
}

Memory::~Memory()
{
}
