#include "SystemGameState.hpp"

#include "SystemObjectFactory.hpp"
#include "SystemGraphics.hpp"
#include "SystemServer.hpp"
#include "MessageObject.hpp"
#include "MessageObjectFactory.hpp"
#include "Object.hpp"
#include "ComponentServerConnection.hpp"
#include "SystemLog.hpp"

#include "OGRE/OgreSceneManager.h"

#include <assert.h>

using namespace ENGINE;

SystemGameState::SystemGameState() :
    System("SystemGameState")
{
    LOG_IN("system");
    mNextPlayerID = 0;
    LOG_OUT("system");
}

SystemGameState::~SystemGameState()
{
    LOG_IN("system");
    LOG_OUT("system");
}

void SystemGameState::init()
{
    LOG_IN("system");
    LOG_OUT("system");
}
    
void SystemGameState::update(float elapsedTime)
{
    LOG_IN_FRAME;
    for(auto i=mNewPlayers.begin(); i!= mNewPlayers.end(); ++i)
    {
        onNewPlayer(*i);
    }
    mNewPlayers.clear();
    LOG_OUT_FRAME;
}

void SystemGameState::receiveMessage(Message *msg)
{
    LOG_IN_MSG;
    LOG_OUT_MSG;
}

ComponentServerConnection *SystemGameState::newPlayer(ParamMap &params)
{
    LOG_IN("system");
    std::string name = "Player" + std::to_string(mNextPlayerID++);
    addObject(Ogre::Vector3(), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), name, "Player");

    Object *newPlayer = SystemObjectFactory::getSingleton()->getObject(name);
    ComponentServerConnection *connection = (ComponentServerConnection *)SystemObjectFactory::getSingleton()->createComponent(newPlayer, "ComponentServerConnection", params);
    mNewPlayers.push_back(connection);

    LOG_OUT("system");
    return connection;
}

void SystemGameState::addObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, std::string type)
{
    LOG_IN("system");
    MessageCreateObject m(pos, ori, parent, name, type + "Server");
    m.sendTo(SystemObjectFactory::getSingleton());

    Object *obj = SystemObjectFactory::getSingleton()->getObject(name);

    assert(obj);

    mLoadedObjects.push_back(std::pair<std::string, Object *>(type, obj));

    MessageCreateObject msg(pos, ori, parent, name, type + "Client");
    SystemServer::getSingleton()->sendToAll(&msg, SystemObjectFactory::getSingleton());
    LOG_OUT("system");
}

void SystemGameState::onNewPlayer(ComponentServerConnection *connection)
{
    LOG_IN("system");

    Object *obj = connection->getObject();

    MessageCreateObject msg(obj->getSceneNode()->getPosition(), obj->getSceneNode()->getOrientation(), obj->getSceneNode()->getParentSceneNode(), obj->getName(), "Player");
    SystemServer::getSingleton()->sendTo(&msg, SystemObjectFactory::getSingleton(), connection);

    StateMap state = obj->getState();
    MessageSetState m(state);
    SystemServer::getSingleton()->sendTo(&m, obj, connection);

    // send objects to new player
    for(auto i=mLoadedObjects.begin(); i!=mLoadedObjects.end(); ++i)
    {
        obj = (*i).second;
        std::string type = (*i).first + "Client";
        if(obj == connection->getObject())
            continue;
        MessageCreateObject msg(obj->getSceneNode()->getPosition(), obj->getSceneNode()->getOrientation(), obj->getSceneNode()->getParentSceneNode(), obj->getName(), type);
        SystemServer::getSingleton()->sendTo(&msg, SystemObjectFactory::getSingleton(), connection);

        StateMap state = obj->getState();
        MessageSetState m(state);
        SystemServer::getSingleton()->sendTo(&m, obj, connection);
    }

    mConnectedPlayers.push_back(connection);
    LOG_OUT("system");
}
