#include <stdio.h>
#include <string>

#include "Engine.hpp"
#include "SystemGraphics.hpp"
#include "SystemObjectFactory.hpp"
#include "SystemPhysics.hpp"
#include "SystemInput.hpp"

#include "ComponentCamera.hpp"
#include "ComponentCollidable.hpp"
#include "ComponentJoystickListener.hpp"
#include "ComponentKeyboardListener.hpp"
#include "ComponentKeyMapping.hpp"
#include "ComponentMouseListener.hpp"
#include "ComponentMover.hpp"
#include "ComponentRenderable.hpp"
#include "ComponentViewport.hpp"

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

        Engine *engine =  new Engine();
        engine->addSystem(SystemGraphics::getSingleton());
        engine->addSystem(SystemObjectFactory::getSingleton());
        engine->addSystem(SystemPhysics::getSingleton());
        engine->addSystem(SystemInput::getSingleton());

        engine->init();

        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0, 0), Ogre::Quaternion(), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "me", "Player");
        SystemObjectFactory::getSingleton()->createObject(Ogre::Vector3(0, 0, -5), Ogre::Quaternion(Ogre::Degree(45), Ogre::Vector3(0,1,0)), SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode(), "cpu0", "CPU");

        engine->run();

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
    }catch(...)
    {
        printf("Unknown Exception!\n");
        return 1;
    }
}
