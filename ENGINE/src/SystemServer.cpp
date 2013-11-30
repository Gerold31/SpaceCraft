#include "SystemServer.hpp"

#include "SystemObjectFactory.hpp"
#include "SystemGraphics.hpp"
#include "SystemConfiguration.hpp"
#include "ComponentServerConnection.hpp"
#include "Object.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

Poco::Net::TCPServerConnection* ServerConnectionFactory::createConnection(const Poco::Net::StreamSocket &socket)
{
    ParamMap params;
    params["Socket"] = socket;

    // @todo let this make SystemGameState
    // @todo give name
    Object *newPlayer = SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "", "PlayerServer");
    ComponentServerConnection *connection = (ComponentServerConnection *)SystemObjectFactory::getSingleton()->createComponent(newPlayer, "ComponentServerConnection", params);
    
    // @todo: inform SystemGameState (todo) that a new Player connected, send him the GameState and inform other connected Player that a new Player connected

    return connection;
}

SystemServer::SystemServer() :
    System("SystemServer")
{
}

SystemServer::~SystemServer()
{
    mServer->stop();
}

void SystemServer::init()
{
    unsigned short port = atoi(SystemConfiguration::getSingleton()->getConfiguration("Port").c_str()); 
    mSocket = new Poco::Net::ServerSocket(port);
    mServer = new Poco::Net::TCPServer(new ServerConnectionFactory(), *mSocket);
    mServer->start();
}

void SystemServer::update(float elapsedTime)
{
}

void SystemServer::receiveMessage(Message *msg)
{
}
