#include "SystemGameState.hpp"

#include "SystemObjectFactory.hpp"
#include "SystemGraphics.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

SystemGameState::SystemGameState() :
    System("SystemGameState")
{
}

SystemGameState::~SystemGameState()
{
}

void SystemGameState::init()
{
}
    
void SystemGameState::update(float elapsedTime)
{
}

void SystemGameState::receiveMessage(Message *msg)
{
}

ComponentServerConnection *SystemGameState::newPlayer(ParamMap &params)
{
    // @todo give name
    Object *newPlayer = SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "", "PlayerServer");
    ComponentServerConnection *connection = (ComponentServerConnection *)SystemObjectFactory::getSingleton()->createComponent(newPlayer, "ComponentServerConnection", params);

    // @todo inform already connected players

    mConnectedPlayers.push_back(connection);

    // @todo send GameState to new Player

    return connection;
}