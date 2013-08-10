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
#include "CPUKeyboard.hpp"
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
        
        SpaceShip *ship = new SpaceShip(455e3, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), Ogre::Quaternion(), engine->getSceneMgr()->getRootSceneNode(), "Ship", engine);
        map->addGravityObject(ship);
        
        Player *player = new Player(Ogre::Vector3(0,1.8,0), Ogre::Quaternion(), ship->getSceneNode(), engine->getWindow(), "Player", engine);
        player->setSpaceShip(ship);

        /*
        Ogre::Light *light = engine->getSceneMgr()->createLight("Light0");
        light->setType(Ogre::Light::LT_DIRECTIONAL);
        light->setDiffuseColour(0.8, 0.8, 0.8);
        light->setSpecularColour(0.8, 0.8, 0.8);
        light->setDirection(1, -2, 1);
        light->setCastShadows(true);
        */

        Ogre::SceneNode *node = ship->getSceneNode()->createChildSceneNode("HumanNode");
        Ogre::Entity *human = engine->getSceneMgr()->createEntity("Human", "Human/Human.mesh");
        node->attachObject(human);
        node->setPosition(0,0.2,0);
        node->setScale(0.1, 0.1, 0.1);

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
