#include "SpaceShip.hpp"

#include "ENGINE.hpp"
#include "Map.hpp"

#include "SpaceShipPartWall.hpp"
#include "SpaceShipPartFloor.hpp"
#include "SpaceShipPartDoor.hpp"
#include "SpaceShipPartLight.hpp"
#include "SpaceShipPartRotatingLight.hpp"
#include "SpaceShipPartWindow.hpp"
#include "Memory.hpp"
#include "CPU.hpp"
#include "CPUDisplay.hpp"
#include "CPUKeyboard.hpp"
#include "CPULightControl.hpp"
#include "CPUDoorControl.hpp"
#include "CPULifeSupport.hpp"
#include "CPULifeDetection.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreStaticGeometry.h"

#include <fstream>

std::string SpaceShip::mType = "SC_SpaceShip";

SpaceShip::SpaceShip(double mass, Ogre::Vector3 velocity, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :GravityObject(mass, velocity, pos, ori, parent, name, mType, engine)
{
    mParts.push_back(new SpaceShipPartFloor(Ogre::Vector3(0,0,0), Ogre::Quaternion(), mNode, name + "Part0", engine));
    mNextPartID = 1;
    mStaticGeometry = engine->getSceneMgr()->createStaticGeometry(name + "Geometry");
}

SpaceShip::~SpaceShip()
{
    mEngine->getSceneMgr()->destroyStaticGeometry(mStaticGeometry);
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
    std::ofstream file(fileName.c_str());
    std::vector<std::pair<CPU *, int> >cpus;
    std::vector<std::pair<CPULightControl *, int> >lightControls;
    std::vector<std::pair<CPUDoorControl *, int> >doorControls;
    for(int i=0; i<mParts.size(); i++)
    {
        file << mParts.at(i)->getType() << std::endl;
        file << Ogre::StringConverter::toString(mParts.at(i)->getParentSceneNode()->getPosition()) << std::endl;
        file << Ogre::StringConverter::toString(mParts.at(i)->getParentSceneNode()->getOrientation()) << std::endl;
        if(mParts.at(i)->getType() == CPU::getType())
            cpus.push_back(std::pair<CPU *, int>((CPU *)mParts.at(i), i));
        else if(mParts.at(i)->getType() == CPULightControl::getType())
            lightControls.push_back(std::pair<CPULightControl *, int>((CPULightControl *)mParts.at(i), i));
        else if(mParts.at(i)->getType() == CPUDoorControl::getType())
            doorControls.push_back(std::pair<CPUDoorControl *, int>((CPUDoorControl *)mParts.at(i), i));
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
    file << std::endl;
    for(int i=0; i<cpus.size(); i++)
    {
        for(int j=0; j<cpus.at(i).first->getNumberDevices(); j++)
        {
            for(int k=0; k<mParts.size(); k++)
            {
                if(mParts.at(k) == cpus.at(i).first->getDevice(j))
                {
                    file << Ogre::StringConverter::toString(Ogre::Vector2(cpus.at(i).second, k)) << std::endl;
                }
            }
        }
    }
    file << std::endl;
    for(int i=0; i<lightControls.size(); i++)
    {
        for(int j=0; j<lightControls.at(i).first->getNumberLights(); j++)
        {
            for(int k=0; k<mParts.size(); k++)
            {
                if(mParts.at(k) == lightControls.at(i).first->getLight(j))
                {
                    file << Ogre::StringConverter::toString(Ogre::Vector2(lightControls.at(i).second, k)) << std::endl;
                }
            }
        }
    }
    file << std::endl;
    for(int i=0; i<doorControls.size(); i++)
    {
        for(int j=0; j<doorControls.at(i).first->getNumberDoors(); j++)
        {
            for(int k=0; k<mParts.size(); k++)
            {
                if(mParts.at(k) == doorControls.at(i).first->getDoor(j))
                {
                    file << Ogre::StringConverter::toString(Ogre::Vector2(doorControls.at(i).second, k)) << std::endl;
                }
            }
        }
    }
}

void SpaceShip::load(std::string fileName)
{
    mStaticGeometry->destroy();
    mStaticGeometry->reset();
    mStaticGeometry->setRegionDimensions(Ogre::Vector3(100, 100, 100));
    mStaticGeometry->setOrigin(Ogre::Vector3(-50, -50, -50));
    mStaticGeometry->setCastShadows(false);
    while(mParts.size())
    {
        mEngine->getMap()->destroyEntity(mParts.back());
        mParts.pop_back();
    }
    std::ifstream file(fileName.c_str());

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

            if(type == SpaceShipPartFloor::getType())
                part = new SpaceShipPartFloor(pos, ori, mNode, /*mStaticGeometry,*/ name, mEngine);
            else if(type == SpaceShipPartWall::getType())
                part = new SpaceShipPartWall(pos, ori, mNode, name, mEngine);
            else if(type == CPU::getType())
            {
                part = new CPU(pos, ori, mNode, name, mEngine);
                Memory *mem = new Memory("program.a", Ogre::Vector3(0,0,0), Ogre::Quaternion(), mNode, "Memory", mEngine);
                ((CPU *)part)->setMemory(mem);
            }else if(type == CPUDisplay::getType())
                part = new CPUDisplay(pos, ori, mNode, name, mEngine);
            else if(type == CPUKeyboard::getType())
                part = new CPUKeyboard(pos, ori, mNode, name, mEngine);
            else if(type == CPUDoorControl::getType())
                part = new CPUDoorControl(pos, ori, mNode, name, mEngine);
            else if(type == CPULifeSupport::getType())
                part = new CPULifeSupport(pos, ori, mNode, name, mEngine);
            else if(type == CPULifeDetection::getType())
                part = new CPULifeDetection(pos, ori, mNode, name, mEngine);
            else if(type == CPULightControl::getType())
                part = new CPULightControl(pos, ori, mNode, name, mEngine);
            else if(type == SpaceShipPartLight::getType())
                part = new SpaceShipPartLight(pos, ori, mNode, name, mEngine);
            else if(type == SpaceShipPartDoor::getType())
                part = new SpaceShipPartDoor(pos, ori, mNode, name, mEngine);
            else if(type == SpaceShipPartRotatingLight::getType())
                part = new SpaceShipPartRotatingLight(pos, ori, mNode, name, mEngine);
            else if(type == SpaceShipPartWindow::getType())
                part = new SpaceShipPartWindow(pos, ori, mNode, name, mEngine);
            else
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
        while(!file.eof())
        {
            Ogre::Vector2 data;
            std::getline(file, line);
            if(line == "")
                break;
            data = Ogre::StringConverter::parseVector2(line);
            ((CPU *)mParts.at(data.x))->addDevice((Hardware *)mParts.at(data.y));
            ((Hardware *)mParts.at(data.y))->connect((CPU *)mParts.at(data.x));
        }
        while(!file.eof())
        {
            Ogre::Vector2 data;
            std::getline(file, line);
            if(line == "")
                break;
            data = Ogre::StringConverter::parseVector2(line);
            ((CPULightControl *)mParts.at(data.x))->addLight((SpaceShipPartLight *)mParts.at(data.y));
            ((SpaceShipPartLight *)mParts.at(data.y))->connect((CPULightControl *)mParts.at(data.x));
        }
        while(!file.eof())
        {
            Ogre::Vector2 data;
            std::getline(file, line);
            if(line == "")
                break;
            data = Ogre::StringConverter::parseVector2(line);
            ((CPUDoorControl *)mParts.at(data.x))->addDoor((SpaceShipPartDoor *)mParts.at(data.y));
            ((SpaceShipPartDoor *)mParts.at(data.y))->connect((CPUDoorControl *)mParts.at(data.x));
        }
    }
    
    for(int i=0; i<mParts.size(); i++)
    {
        if(mParts.at(i)->getType() == "CPU")
            ((CPU *)mParts.at(i))->start();
    }

    //mStaticGeometry->build();

    if(mParts.size() == 0)
        mParts.push_back(new SpaceShipPartFloor(Ogre::Vector3(0,0,0), Ogre::Quaternion(), mNode, mName + "Part0", mEngine));
    mNextPartID = mParts.size();

}
