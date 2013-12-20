#include "SystemGraphics.hpp"

#include "SystemConfiguration.hpp"
#include "SystemLog.hpp"

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreConfigFile.h"

#include <vector>
#include <exception>

using namespace ENGINE;

SystemGraphics::SystemGraphics() :
    System("SystemGraphics")
{
    LOG_IN("system");
    try{
        mRoot = new Ogre::Root(Ogre::String("plugins").append(OGRE_DEBUG_MODE ? "_d.cfg" : ".cfg"), "", "Ogre.log");

        mWindow = nullptr;
        
        if(SystemConfiguration::getSingleton()->getConfiguration("Rendering") == "true")
        {

            const Ogre::RenderSystemList& renderSystemList = mRoot->getAvailableRenderers();
            if( renderSystemList.size() == 0 )
            {
                LOG("Sorry, no rendersystem was found.", "error");
                LOG_OUT("system");
                return;
            }

            Ogre::RenderSystem *renderSystem = renderSystemList.at(0);
            mRoot->setRenderSystem(renderSystem);

            mRoot->initialise(false, "", "");

            Ogre::NameValuePairList params;
            params["FSAA"] = "0"; 
            params["vsync"] = "true";
            mWindow = mRoot->createRenderWindow("ENGINE", 800, 600, false, &params);

            mWindow->setActive(true);
            mWindow->setAutoUpdated(false);

        }

        mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "SceneMgr");
        mRootSceneNode = mSceneMgr->getRootSceneNode();
        
        if(mWindow)
        {

            // set up resources
            // Load resource paths from config file
            Ogre::ConfigFile cf;

            cf.load("resources.cfg");
 
            // Go through all sections & settings in the file
            Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
            Ogre::String secName, typeName, archName;
            while (seci.hasMoreElements())
            {
                secName = seci.peekNextKey();
                Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
                Ogre::ConfigFile::SettingsMultiMap::iterator i;
                for (i = settings->begin(); i != settings->end(); ++i)
                {
                    typeName = i->first;
                    archName = i->second;
                    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
                }
            }
            Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        }

        mSceneMgr->setAmbientLight(Ogre::ColourValue(0,0,0));
        mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
        mSceneMgr->setShadowFarDistance(500);

        mRoot->clearEventTimes();

    }catch(Ogre::Exception &e)
    {
        LOG(e.what(), "error");
    }catch(std::exception &e)
    {
        LOG(e.what(), "error");
    }catch(...)
    {
        LOG("Unknown Error", "error");
    }
    LOG_OUT("system");
}

SystemGraphics::~SystemGraphics()
{
    LOG_IN("system");
    /*
    if(mWindow){
        mWindow->destroy();
        delete mWindow;
        mWindow = NULL;
    }
    if(mRoot)
    {
        mRoot->shutdown();
        mRoot = NULL;
    }
    */
    LOG_OUT("system");
}

void SystemGraphics::init()
{
    LOG_IN("system");
    LOG_OUT("system");
}
    
void SystemGraphics::update(float elapsedTime)
{
    if(mWindow)
    {
        mWindow->update(false);
 
        mWindow->swapBuffers();

        mRoot->renderOneFrame();
    }
 
    Ogre::WindowEventUtilities::messagePump();
}

void SystemGraphics::receiveMessage(Message *msg)
{
}
