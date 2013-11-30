#include <stdio.h>
#include <string>

#include "Engine.hpp"
#include "SystemGraphics.hpp"
#include "SystemObjectFactory.hpp"
#include "SystemPhysics.hpp"
#include "SystemInput.hpp"
#include "SystemClient.hpp"
#include "SystemServer.hpp"
#include "SystemConfiguration.hpp"

#include "ComponentCamera.hpp"
#include "ComponentCollidable.hpp"
#include "ComponentJoystickListener.hpp"
#include "ComponentKeyboardListener.hpp"
#include "ComponentKeyMapping.hpp"
#include "ComponentMouseListener.hpp"
#include "ComponentMover.hpp"
#include "ComponentRenderable.hpp"
#include "ComponentViewport.hpp"
#include "ComponentServerConnection.hpp"

#include "NetworkMessage.hpp"
#include "MessageEngine.hpp"
#include "MessageMove.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;

int main(int argc, char **argv)
{
    printf("Start\n");
    try{

        SystemObjectFactory::getSingleton()->registerComponent(ComponentCamera::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentCollidable::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentJoystickListener::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentKeyboardListener::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentKeyMapping::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentMouseListener::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentMover::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentRenderable::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentViewport::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentServerConnection::getType());
        
        NetworkMessage::registerMessge(MessageQuit::getID(), MessageQuit::CreateMessage);
        NetworkMessage::registerMessge(MessageStartMoveForward::getID(), MessageStartMoveForward::CreateMessage);
        NetworkMessage::registerMessge(MessageStopMoveForward::getID(), MessageStopMoveForward::CreateMessage);
        NetworkMessage::registerMessge(MessageStartMoveBackward::getID(), MessageStartMoveBackward::CreateMessage);
        NetworkMessage::registerMessge(MessageStopMoveBackward::getID(), MessageStopMoveBackward::CreateMessage);
        NetworkMessage::registerMessge(MessageStartMoveLeft::getID(), MessageStartMoveLeft::CreateMessage);
        NetworkMessage::registerMessge(MessageStopMoveLeft::getID(), MessageStopMoveLeft::CreateMessage);
        NetworkMessage::registerMessge(MessageStartMoveRight::getID(), MessageStartMoveRight::CreateMessage);
        NetworkMessage::registerMessge(MessageStopMoveRight::getID(), MessageStopMoveRight::CreateMessage);
        NetworkMessage::registerMessge(MessageLookAtRel::getID(), MessageLookAtRel::CreateMessage);
        
        Engine::getSingleton()->addSystem(SystemConfiguration::getSingleton());

        if(argc == 2)
            SystemConfiguration::getSingleton()->loadFromFile(argv[1]);
        else
            SystemConfiguration::getSingleton()->loadFromFile("SpaceCraftListenServer.cfg");

        if(SystemConfiguration::getSingleton()->getConfiguration("NetworkType") == "Client")
        {
            Engine::getSingleton()->addSystem(SystemGraphics::getSingleton());
            Engine::getSingleton()->addSystem(SystemObjectFactory::getSingleton());
            Engine::getSingleton()->addSystem(SystemPhysics::getSingleton());
            Engine::getSingleton()->addSystem(SystemInput::getSingleton());

            Engine::getSingleton()->addSystem(SystemClient::getSingleton());
        }else if(SystemConfiguration::getSingleton()->getConfiguration("NetworkType") == "DedicatedServer")
        {
            Engine::getSingleton()->addSystem(SystemGraphics::getSingleton());
            Engine::getSingleton()->addSystem(SystemObjectFactory::getSingleton());
            Engine::getSingleton()->addSystem(SystemPhysics::getSingleton());

            Engine::getSingleton()->addSystem(SystemServer::getSingleton());
        }else if(SystemConfiguration::getSingleton()->getConfiguration("NetworkType") == "ListenServer")
        {
            Engine::getSingleton()->addSystem(SystemGraphics::getSingleton());
            Engine::getSingleton()->addSystem(SystemObjectFactory::getSingleton());
            Engine::getSingleton()->addSystem(SystemPhysics::getSingleton());
            Engine::getSingleton()->addSystem(SystemInput::getSingleton());
            
            Engine::getSingleton()->addSystem(SystemServer::getSingleton());
            Engine::getSingleton()->addSystem(SystemClient::getSingleton());
        }else
        {
            throw "No NetworkType specified";
        }
        
        Engine::getSingleton()->init();

        printf("init world\n");
        // @todo remove, create onConnect instead
        if(SystemConfiguration::getSingleton()->isClient())
        {
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "me", "PlayerClient");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu1c", "CPUClient");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0,  5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu2c", "CPUClient");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(-5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu3c", "CPUClient");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3( 5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu4c", "CPUClient");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, -5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu5c", "CPUClient");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0,  5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu6c", "CPUClient");
        }

        // @todo use messages
        if(SystemConfiguration::getSingleton()->isServer())
        {
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu1", "CPUServer");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0,  5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu2", "CPUServer");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(-5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu3", "CPUServer");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3( 5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu4", "CPUServer");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, -5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu5", "CPUServer");
            SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0,  5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu6", "CPUServer");
        }
        printf("init finished\n");

        Engine::getSingleton()->run();

        printf("End\n");
        return 0;
    }catch(const char *msg)
    {
        printf("%s\n", msg);
        return 1;
    }catch(std::string msg)
    {
        printf("%s\n", msg.c_str());
        return 1;
    }catch(Ogre::Exception &e)
    {
        printf("%s\n", e.what());
        return 1;
    }catch(...)
    {
        printf("Unknown Exception!\n");
        return 1;
    }
}
