#include "SpaceShip.hpp"

#include "ENGINE.hpp"
#include "Map.hpp"

#include "SpaceShipPart.hpp"
#include "CPU.hpp"
#include "CPUDisplay.hpp"
#include "CPUKeyboard.hpp"
#include "SpaceShipPartRotatingLight.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"

#include <fstream>

SpaceShip::SpaceShip(double mass, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :GravityObject(mass, velocity, pos, ori, parent, name, "SC_SpaceShip", engine)
{
    mParts.push_back(new SpaceShipPart(SpaceShipPart::PART_FLOOR, true, Ogre::Vector3(0,0,0), Ogre::Quaternion(), mNode, name + "Part0", "SC_SpaceShipPartFloor", engine));
    mNextPartID = 1;
}

void SpaceShip::addPart(SpaceShipPart *newPart)
{
    mParts.push_back(newPart);
    mNextPartID++;
}

void SpaceShip::removePart(SpaceShipPart *part)
{
    for(std::vector<SpaceShipPart *>::reverse_iterator i = mParts.rbegin(); i != mParts.rend(); ++i)
    {
        if((*i) == part)
        {
            mParts.erase(--(i.base()));
            break;
        }
    }
}

void SpaceShip::save(std::string fileName)
{
    std::ofstream file(fileName);
    for(int i=0; i<mParts.size(); i++)
    {
        file << mParts.at(i)->getType() << std::endl;
        file << Ogre::StringConverter::toString(mParts.at(i)->getSceneNode()->getPosition()) << std::endl;
        file << Ogre::StringConverter::toString(mParts.at(i)->getSceneNode()->getOrientation()) << std::endl;
    }
    file << std::endl;
    for(int i=0; i<mParts.size(); i++)
    {
        for(int j=0; j<mParts.at(i)->getNumberNeighbors(); j++)
        {
            if(mParts.at(i)->getNeighbor(j))
            {
                for(int k=0; k<mParts.size(); k++)
                {
                    if(mParts.at(i)->getNeighbor(j) == mParts.at(k))
                        file << Ogre::StringConverter::toString(Ogre::Vector3(i, j, k)) << std::endl;
                }
            }
        }
    }
}

void SpaceShip::load(std::string fileName)
{
    while(mParts.size())
    {
        mEngine->getMap()->destroyEntity(mParts.back());
        mParts.pop_back();
    }
    std::ifstream file(fileName);

    if(file.is_open())
    {
        std::string line;
        for(int i=0; ;i++)
        {
            std::string type;
            std::getline(file, type);
            if(type == "")
                break;
            char name[32];
            sprintf(name, "%sPart%d", mName.c_str(), i);

            SpaceShipPart *part = NULL;
            Ogre::Vector3 pos;
            Ogre::Quaternion ori;
            std::getline(file, line);
            pos = Ogre::StringConverter::parseVector3(line);
            std::getline(file, line);
            ori = Ogre::StringConverter::parseQuaternion(line);


            if(type == "SC_SpaceShipPartFloor")
            {
                part = new SpaceShipPart(SpaceShipPart::PART_FLOOR, false, pos, ori, mNode, name, type, mEngine);
            }else if(type == "SC_SpaceShipPartWall")
            {
                part = new SpaceShipPart(SpaceShipPart::PART_WALL, false, pos, ori, mNode, name, type, mEngine);
            }else if(type == "SC_CPU")
            {
                part = new CPU(pos, ori, mNode, name, mEngine);
            }else if(type == "CPU_Display")
            {
                part = new CPUDisplay(pos, ori, mNode, name, mEngine);
            }else if(type == "CPU_Keyboard")
            {
                part = new CPUKeyboard(pos, ori, mNode, name, mEngine);
            }else if(type == "SC_SpaceShipPartLight")
            {
                part = new SpaceShipPartRotatingLight(pos, ori, mNode, name, mEngine);
            }else
                printf("Error loading %s: invalid type %s\n", fileName.c_str(), type.c_str());
            mParts.push_back(part);
        }
        while(!file.eof())
        {
            Ogre::Vector3 data;
            std::getline(file, line);
            if(line == "")
                break;
            data = Ogre::StringConverter::parseVector3(line);
            mParts.at(data.x)->setNeighbor(mParts.at(data.z), data.y);
        }
    }
    if(mParts.size() == 0)
        mParts.push_back(new SpaceShipPart(SpaceShipPart::PART_FLOOR, true, Ogre::Vector3(0,0,0), Ogre::Quaternion(), mNode, mName + "Part0", "SC_SpaceShipPartFloor", mEngine));
    mNextPartID = mParts.size();
}