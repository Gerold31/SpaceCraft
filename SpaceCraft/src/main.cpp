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
#include "SystemGUI.hpp"
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
#include "ComponentUse.hpp"
#include "ComponentInventory.hpp"
#include "ComponentInventoryGUI.hpp"
#include "ComponentItem.hpp"
#include "ComponentMultiUse.hpp"
#include "ComponentHotbar.hpp"
#include "ComponentHotbarGUI.hpp"

#include "ComponentCPU.hpp"
#include "ComponentMemory.hpp"
#include "ComponentHardwareDisplay.hpp"
#include "ComponentHardwareKeyboard.hpp"
#include "ComponentSpaceShipPartFloor.hpp"
#include "ComponentSpaceShipPartFloorMount.hpp"
#include "ComponentSpaceShipPartCeilMount.hpp"
#include "ComponentSpaceShipPartWall.hpp"
#include "ComponentSpaceShipPartWallMount.hpp"

#include "Message.hpp"
#include "MessageEngine.hpp"
#include "MessageMove.hpp"
#include "MessageObject.hpp"
#include "MessageObjectFactory.hpp"
#include "MessageUse.hpp"
#include "MessageObject.hpp"
#include "MessageInventory.hpp"
#include "MessageHotbar.hpp"

#include "MessageCPU.hpp"
#include "MessageHardwareKeyboard.hpp"
#include "MessageHardwareDisplay.hpp"

#include "Object.hpp"

#include "OGRE/OgreSceneManager.h"

#include <signal.h>

void sigHandler(int param)
{
    if(param == SIGSEGV)
    {
        LOG("Segfault", "error");
        exit(1);
    }
}

using namespace ENGINE;
using namespace SpaceCraft;

int main(int argc, char **argv)
{
    LOG_IN("log");
    signal(SIGSEGV, sigHandler);
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
        SystemObjectFactory::getSingleton()->registerComponent(ComponentUse::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentInventory::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentInventoryGUI::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentItem::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentMultiUse::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentHotbar::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentHotbarGUI::getType());
        
        SystemObjectFactory::getSingleton()->registerComponent(ComponentCPU::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentMemory::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentHardwareDisplay::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentHardwareKeyboard::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentSpaceShipPartFloor::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentSpaceShipPartFloorMount::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentSpaceShipPartCeilMount::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentSpaceShipPartWall::getType());
        SystemObjectFactory::getSingleton()->registerComponent(ComponentSpaceShipPartWallMount::getType());
        

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
        Message::registerMessge(MessageUse::getID(), MessageUse::CreateMessage);
        Message::registerMessge(MessageEnable::getID(), MessageEnable::CreateMessage);
        Message::registerMessge(MessageDisable::getID(), MessageDisable::CreateMessage);
        Message::registerMessge(MessageEnableComponent::getID(), MessageEnableComponent::CreateMessage);
        Message::registerMessge(MessageDisableComponent::getID(), MessageDisableComponent::CreateMessage);
        Message::registerMessge(MessageToggleInventory::getID(), MessageToggleInventory::CreateMessage);
        Message::registerMessge(MessageMultiUse::getID(), MessageMultiUse::CreateMessage);
        Message::registerMessge(MessageRegisterMultiUse::getID(), MessageRegisterMultiUse::CreateMessage);
        Message::registerMessge(MessageRequestMultiUse::getID(), MessageRequestMultiUse::CreateMessage);
        Message::registerMessge(MessageSetState::getID(), MessageSetState::CreateMessage);
        Message::registerMessge(MessageOnUse::getID(), MessageOnUse::CreateMessage);
        Message::registerMessge(MessageOnMultiUse::getID(), MessageOnMultiUse::CreateMessage);
        Message::registerMessge(MessageNextSlot::getID(), MessageNextSlot::CreateMessage);
        Message::registerMessge(MessagePrevSlot::getID(), MessagePrevSlot::CreateMessage);
        Message::registerMessge(MessageSelectHotbarItem::getID(), MessageSelectHotbarItem::CreateMessage);
        Message::registerMessge(MessageSlot0::getID(), MessageSlot0::CreateMessage);
        Message::registerMessge(MessageSlot1::getID(), MessageSlot1::CreateMessage);
        Message::registerMessge(MessageSlot2::getID(), MessageSlot2::CreateMessage);
        Message::registerMessge(MessageSlot3::getID(), MessageSlot3::CreateMessage);
        Message::registerMessge(MessageSlot4::getID(), MessageSlot4::CreateMessage);
        Message::registerMessge(MessageSlot5::getID(), MessageSlot5::CreateMessage);
        Message::registerMessge(MessageSlot6::getID(), MessageSlot6::CreateMessage);
        Message::registerMessge(MessageSlot7::getID(), MessageSlot7::CreateMessage);
        Message::registerMessge(MessageSlot8::getID(), MessageSlot8::CreateMessage);
        Message::registerMessge(MessageSlot9::getID(), MessageSlot9::CreateMessage);

        Message::registerMessge(MessageInterrupt::getID(), MessageInterrupt::CreateMessage);
        Message::registerMessge(MessageHardwareKeyPressed::getID(), MessageHardwareKeyPressed::CreateMessage);
        Message::registerMessge(MessageHardwareKeyReleased::getID(), MessageHardwareKeyReleased::CreateMessage);
        Message::registerMessge(MessageDisplaySetImage::getID(), MessageDisplaySetImage::CreateMessage);
        

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
            Engine::getSingleton()->addSystem(SystemGUI::getSingleton());
        }

        SystemLog::getSingleton()->createLog(SystemConfiguration::getSingleton()->getConfiguration("NetworkType") + ".html");

        Engine::getSingleton()->init();

        LOG("init world", "log");

        if(SystemConfiguration::getSingleton()->isClient())
        {
        }

        if(SystemConfiguration::getSingleton()->isServer())
        {
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(0, 0, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu", "CPU");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(5, 0, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "memory", "Memory");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(0, 2, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "display", "HardwareDisplay");
            SystemGameState::getSingleton()->addObject(Ogre::Vector3(-2, 0, -5), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "keyboard", "HardwareKeyboard");

            Object *cpuo = SystemObjectFactory::getSingleton()->getObject("cpu");
            Object *memoryo = SystemObjectFactory::getSingleton()->getObject("memory");
            Object *displayo = SystemObjectFactory::getSingleton()->getObject("display");
            Object *keyboardo = SystemObjectFactory::getSingleton()->getObject("keyboard");

            assert(cpuo);
            assert(memoryo);
            assert(displayo);
            assert(keyboardo);

            ComponentCPU *cpuc;
            ComponentMemory *memoryc;
            ComponentHardwareDisplay *displayc;
            ComponentHardwareKeyboard *keyboardc;

            for(int i=0; i<cpuo->getNumberComponents(); i++)
            {
                Component *c = cpuo->getComponent(i);
                if(c->getType() == ComponentCPU::getType())
                {
                    cpuc = (ComponentCPU *)c;
                    break;
                }
            }
            for(int i=0; i<memoryo->getNumberComponents(); i++)
            {
                Component *c = memoryo->getComponent(i);
                if(c->getType() == ComponentMemory::getType())
                {
                    memoryc = (ComponentMemory *)c;
                    break;
                }
            }
            for(int i=0; i<displayo->getNumberComponents(); i++)
            {
                Component *c = displayo->getComponent(i);
                if(c->getType() == ComponentHardwareDisplay::getType())
                {
                    displayc = (ComponentHardwareDisplay *)c;
                    break;
                }
            }
            for(int i=0; i<keyboardo->getNumberComponents(); i++)
            {
                Component *c = keyboardo->getComponent(i);
                if(c->getType() == ComponentHardwareKeyboard::getType())
                {
                    keyboardc = (ComponentHardwareKeyboard *)c;
                    break;
                }
            }

            assert(cpuc);
            assert(memoryc);
            assert(displayc);
            assert(keyboardc);

            cpuc->setMemory(memoryc);
            cpuc->addDevice(displayc);
            displayc->connect(cpuc);
            cpuc->addDevice(keyboardc);
            keyboardc->connect(cpuc);

            //cpuc->start();

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
