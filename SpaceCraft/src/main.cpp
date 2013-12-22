#include <string>

#include "Engine.hpp"
#include "SystemGraphics.hpp"
#include "SystemObjectFactory.hpp"
#include "SystemPhysics.hpp"
#include "SystemInput.hpp"
#include "SystemClient.hpp"
#include "SystemServer.hpp"
#include "SystemConfiguration.hpp"
#include "SystemGameState.hpp"
#include "SystemLog.hpp"

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

#include "ComponentCPU.hpp"
#include "ComponentMemory.hpp"

#include "Message.hpp"
#include "MessageEngine.hpp"
#include "MessageMove.hpp"
#include "MessageObjectFactory.hpp"

#include "OGRE/OgreSceneManager.h"

using namespace ENGINE;
using namespace SpaceCraft;

int main(int argc, char **argv)
{
    LOG_IN("log");
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

        SystemObjectFactory::getSingleton()->registerComponent(ComponentCPU::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentMemory::getType());
        
        Message::registerMessge(MessageQuit::getID(), MessageQuit::CreateMessage);
        Message::registerMessge(MessageStartMoveForward::getID(), MessageStartMoveForward::CreateMessage);
        Message::registerMessge(MessageStopMoveForward::getID(), MessageStopMoveForward::CreateMessage);
        Message::registerMessge(MessageStartMoveBackward::getID(), MessageStartMoveBackward::CreateMessage);
        Message::registerMessge(MessageStopMoveBackward::getID(), MessageStopMoveBackward::CreateMessage);
        Message::registerMessge(MessageStartMoveLeft::getID(), MessageStartMoveLeft::CreateMessage);
        Message::registerMessge(MessageStopMoveLeft::getID(), MessageStopMoveLeft::CreateMessage);
        Message::registerMessge(MessageStartMoveRight::getID(), MessageStartMoveRight::CreateMessage);
        Message::registerMessge(MessageStopMoveRight::getID(), MessageStopMoveRight::CreateMessage);
        Message::registerMessge(MessageLookAtRel::getID(), MessageLookAtRel::CreateMessage);
        Message::registerMessge(MessageCreateObject::getID(), MessageCreateObject::CreateMessage);
        Message::registerMessge(MessageSetPosition::getID(), MessageSetPosition::CreateMessage);
        
        Engine::getSingleton()->addSystem(SystemConfiguration::getSingleton());

        if(argc == 2)
            SystemConfiguration::getSingleton()->loadFromFile(argv[1]);
        else
            SystemConfiguration::getSingleton()->loadFromFile("SpaceCraftClient.cfg");

        Engine::getSingleton()->addSystem(SystemGraphics::getSingleton());
        Engine::getSingleton()->addSystem(SystemObjectFactory::getSingleton());
        Engine::getSingleton()->addSystem(SystemPhysics::getSingleton());
        Engine::getSingleton()->addSystem(SystemLog::getSingleton());

        if(SystemConfiguration::getSingleton()->isServer())
        {
            Engine::getSingleton()->addSystem(SystemGameState::getSingleton());
            Engine::getSingleton()->addSystem(SystemServer::getSingleton());
        }

        if(SystemConfiguration::getSingleton()->isClient())
        {
            Engine::getSingleton()->addSystem(SystemInput::getSingleton());
            Engine::getSingleton()->addSystem(SystemClient::getSingleton());
        }

        SystemLog::getSingleton()->createLog(SystemConfiguration::getSingleton()->getConfiguration("NetworkType") + ".html");

        Engine::getSingleton()->init();

        LOG("init world", "log");

        if(SystemConfiguration::getSingleton()->isClient())
        {
        }

        if(SystemConfiguration::getSingleton()->isServer())
        {
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(0, 0, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu1", "CPU");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(0, 0,  5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu2", "CPU");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(-5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu3", "CPU");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3( 5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu4", "CPU");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(0, -5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu5", "CPU");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(0,  5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu6", "CPU");
        }
        LOG("init finished", "log");

        Engine::getSingleton()->run();

        LOG_OUT("log");
        SystemLog::getSingleton()->close();
        return 0;
    }catch(const char *msg)
    {
        LOG(msg, "error");
    }catch(std::string msg)
    {
        LOG(msg, "error");
    }catch(Ogre::Exception &e)
    {
        LOG(e.what(), "error");
    }catch(std::exception &e)
    {
        LOG(e.what(), "error");
    }catch(...)
    {
        LOG("Unknown Exception!", "error");
    }
    LOG_OUT("log");
    SystemLog::getSingleton()->close();
    return 1;
}
