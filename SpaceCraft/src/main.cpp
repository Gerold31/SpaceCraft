#include <stdio.h>
#include <string>

#include "Engine.hpp"
#include "SystemGraphics.hpp"
#include "SystemObjectFactory.hpp"
#include "SystemPhysics.hpp"
#include "SystemInput.hpp"
#include "SystemClient.hpp"
#include "SystemServer.hpp"

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
        
        Engine::getSingleton()->addSystem(SystemGraphics::getSingleton());
        Engine::getSingleton()->addSystem(SystemObjectFactory::getSingleton());
        Engine::getSingleton()->addSystem(SystemPhysics::getSingleton());
        Engine::getSingleton()->addSystem(SystemInput::getSingleton());

        //Engine::getSingleton()->addSystem(SystemClient::getSingleton());
        Engine::getSingleton()->addSystem(SystemServer::getSingleton());

        Engine::getSingleton()->init();

        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "me", "Player");

        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu1", "CPU");
        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0,  5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu2", "CPU");
        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(-5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu3", "CPU");
        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3( 5, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu4", "CPU");
        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, -5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu5", "CPU");
        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0,  5, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu6", "CPU");

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
