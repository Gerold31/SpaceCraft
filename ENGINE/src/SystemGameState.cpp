#include "SystemGameState.hpp"

#include "SystemObjectFactory.hpp"
#include "SystemGraphics.hpp"
#include "SystemServer.hpp"
#include "MessageObjectFactory.hpp"
#include "Object.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

SystemGameState::SystemGameState() :
    System("SystemGameState")
{
    mNextPlayerID = 0;
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
    std::string name = "Player" + std::to_string(mNextPlayerID++);
    addObject(Ogre::Vector3(), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), name, "Player");

    Object *newPlayer = SystemObjectFactory::getSingleton()->getObject(name);
    ComponentServerConnection *connection = (ComponentServerConnection *)SystemObjectFactory::getSingleton()->createComponent(newPlayer, "ComponentServerConnection", params);
    
    for(auto i=mLoadedObjects.begin(); i!=mLoadedObjects.end(); ++i)
    {
        Object *obj = (*i).second;
        std::string type = (*i).first + "Client";
        if(obj->getName() == name)
            type = "Player";
        MessageCreateObject msg(obj->getSceneNode()->getPosition(), obj->getSceneNode()->getOrientation(), obj->getSceneNode()->getParentSceneNode(), obj->getName(), type);
        SystemServer::getSingleton()->sendTo(&msg, SystemObjectFactory::getSingleton(), connection);
    }

    mConnectedPlayers.push_back(connection);

    return connection;
}

void SystemGameState::addObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, std::string type)
{
    MessageCreateObject m(pos, ori, parent, name, type + "Server");
    m.sendTo(SystemObjectFactory::getSingleton());

    Object *obj = SystemObjectFactory::getSingleton()->getObject(name);

    mLoadedObjects.push_back(std::pair<std::string, Object *>(type, obj));

    MessageCreateObject msg(pos, ori, parent, name, type + "Client");
    SystemServer::getSingleton()->sendToAll(&msg, SystemObjectFactory::getSingleton());
}