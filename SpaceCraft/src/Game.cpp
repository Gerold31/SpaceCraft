#include "Game.hpp"

#include "OGRE/OgreSceneManager.h"

#include "ENGINE.hpp"

#include "SpaceShip.hpp"
#include "Player.hpp"
#include "Bot.hpp"
#include "CPU.hpp"
#include "CPUDoorControl.hpp"
#include "CPULifeDetection.hpp"
#include "CPULifeSupport.hpp"

Game::Game(Difficulty difficulty, ENGINE *engine)
    : Map(engine)
{
    mDifficulty = difficulty;
}

Game::~Game()
{
}

void Game::init()
{
    Map::init();

    SpaceShip *ship = new SpaceShip(455e3, Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), Ogre::Quaternion(), mEngine->getSceneMgr()->getRootSceneNode(), "Ship", mEngine);
    
    //ship->load("Ship.ship");

    Player *player = new Player(Ogre::Vector3(0,1.67,0), Ogre::Quaternion(), ship->getParentSceneNode(), mEngine->getWindow(), "Player", mEngine);
    player->setSpaceShip(ship);

    /*
    Ogre::Vector2 spawnZones[5][2] = {{Ogre::Vector2(-27.6, 15.6), Ogre::Vector2(3.6, 2.4)},{Ogre::Vector2(24.0, 15.6), Ogre::Vector2(3.6, 2.4)},
                                      {Ogre::Vector2(-27.6, -4.8), Ogre::Vector2(3.6, 2.4)},{Ogre::Vector2(24.0, -4.8), Ogre::Vector2(3.6, 2.4)},
                                      {Ogre::Vector2(-1.2, 33.6), Ogre::Vector2(2.4, 3.6)}};

    int numberBots = 5 + rand()%5;
    for(int i=0; i<numberBots; i++)
    {
        int zone = rand()%5;
        float offX = 1.2*(rand()%int(spawnZones[zone][1].x/1.2));
        float offY = 1.2*(rand()%int(spawnZones[zone][1].y/1.2));
        char name[6];
        sprintf(name, "Bot%d", i);
        new Bot(Ogre::Vector3(spawnZones[zone][0].x + offX, 1.67, spawnZones[zone][0].y + offY), Ogre::Quaternion(), ship->getParentSceneNode(), name, mEngine);

        if(i<=mDifficulty)
            Bot::addTask(new Bot::Task(Bot::Task::TASK_TYPE_KILL, (DIFFICULTY_SUIZIDAL-mDifficulty)*(rand()%5 + 1), player));
    }

    for(int i=0; i<ship->getNumberOfParts(); i++)
    {
        SpaceShipPart *part = ship->getPart(i);
        if(part->getType() == CPU::getType())
        {
            Bot::addTask(new Bot::Task(Bot::Task::TASK_TYPE_DESTROY, 1+rand()%5, part));
        }else if(part->getType() == CPUDoorControl::getType())
        {
            Bot::addTask(new Bot::Task(Bot::Task::TASK_TYPE_DESTROY, 3+rand()%5, part));
        }else if(part->getType() == CPULifeDetection::getType())
        {
            Bot::addTask(new Bot::Task(Bot::Task::TASK_TYPE_DESTROY, 0+rand()%3, part));
        }else if(part->getType() == CPULifeSupport::getType())
        {
            if(mDifficulty == DIFFICULTY_SUIZIDAL)
                Bot::addTask(new Bot::Task(Bot::Task::TASK_TYPE_DESTROY, 1, part));
        }
    }
    */
}

bool Game::update(float elapsedTime)
{
    return Map::update(elapsedTime);
}