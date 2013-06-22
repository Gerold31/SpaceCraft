#include "ENGINE.hpp"

#include "OGRE/OgreVector3.h"
#include "OGRE/OgreQuaternion.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreSceneNode.h"

#include "Player.hpp"
#include "KinoControl.hpp"
#include "CPU.hpp"
#include "Memory.hpp"
#include "CPUDisplay.hpp"
#include "Star.hpp"
#include "SpaceShip.hpp"
#include "Map.hpp"
#include "Universe.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <stdio.h>

#define ENGINE_ERROR(x) printf(x);

int main(int argc, char **argv)
{
    try{
        ENGINE *engine = new ENGINE();
        Universe *map = new Universe(engine);
        

        if(!engine->init(map))
        {
            ENGINE_ERROR("failed to initialize ENGINE");
            return -1;
        }

        new Star(Ogre::Vector3(0, 0, -5000), Ogre::Quaternion(), engine->getSceneMgr()->getRootSceneNode(), "Star", engine);
        SpaceShip *ship = new SpaceShip(Ogre::Vector3(0,0,0), Ogre::Quaternion(), engine->getSceneMgr()->getRootSceneNode(), "Ship", engine);

        Player *player = new Player(Ogre::Vector3(0,1.8,0), Ogre::Quaternion(), ship->getSceneNode(), engine->getWindow(), "Player", engine);
        player->setSpaceShip(ship);
    
        new KinoControl(Ogre::Vector3(0, 10, -10), Ogre::Quaternion(), ship->getSceneNode(), "KinoControl0", engine);
        new KinoControl(Ogre::Vector3(0, 0, -10), Ogre::Quaternion(), ship->getSceneNode(), "KinoControl1", engine);

        Memory *mem = new Memory("program.a", Ogre::Vector3(10, 15, -10), Ogre::Quaternion(), ship->getSceneNode(), "program", engine);
        CPU    *cpu = new CPU   (Ogre::Vector3(10, 10, -10), Ogre::Quaternion(), ship->getSceneNode(), "CPU", engine); 
    
        CPUDisplay *display = new CPUDisplay(Ogre::Vector3(-10, 10, -10), Ogre::Quaternion(), ship->getSceneNode(), "Display", engine);

        new CPUDisplay(Ogre::Vector3(-10, 0, -10), Ogre::Quaternion(), ship->getSceneNode(), "Display2", engine);

        cpu->addDevice(display);
        display->connect(cpu);

        cpu->setMemory(mem);
        cpu->start();

        /*
        Ogre::SceneNode *node = engine->getSceneMgr()->getRootSceneNode()->createChildSceneNode("box1");
        Ogre::Entity * ent = engine->getSceneMgr()->createEntity("box1ent", "SpaceShip/Part_Floor.mesh");
        node->attachObject(ent);
        node->setPosition(Ogre::Vector3(0, 0, -20));

        node = engine->getSceneMgr()->getRootSceneNode()->createChildSceneNode("box2");
        ent = engine->getSceneMgr()->createEntity("box2ent", "SpaceShip/Part_Floor.mesh");
        node->attachObject(ent);
        node->setPosition(Ogre::Vector3(2, 2, -20));
        */

        engine->run();

        delete engine;
        engine = NULL;
    }catch(Ogre::Exception &e)
    {
        ENGINE_ERROR(e.what());
    }catch(std::exception &e)
    {
        ENGINE_ERROR(e.what());
    }catch(...)
    {
        ENGINE_ERROR("Unknown Error");
    }
    return 0;
}