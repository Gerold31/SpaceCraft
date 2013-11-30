#include "SystemGraphics.hpp"

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreConfigFile.h"

#include <vector>
#include <exception>
#include <stdio.h>

using namespace ENGINE;

#define ENGINE_ERROR(str) printf("%s", str)

SystemGraphics::SystemGraphics() :
    System("SystemGraphics")
{
    try{
        mRoot = new Ogre::Root(Ogre::String("plugins").append(OGRE_DEBUG_MODE ? "_d.cfg" : ".cfg"), "", "Ogre.log");

        /*
        std::vector<Ogre::String> plugins;

        plugins.push_back("RenderSystem_GL");
        //plugins.push_back("RenderSystem_Direct3D9");
        //plugins.push_back("RenderSystem_Direct3D11");
        plugins.push_back("Plugin_ParticleFX");
        plugins.push_back("Plugin_CgProgramManager");
        plugins.push_back("Plugin_OctreeSceneManager");

        while(plugins.size())
        {
            mRoot->loadPlugin(plugins.back().append(OGRE_DEBUG_MODE ? "_d" : ""));
            plugins.pop_back();
        }
        */
    
        const Ogre::RenderSystemList& renderSystemList = mRoot->getAvailableRenderers();
        if( renderSystemList.size() == 0 )
        {
            ENGINE_ERROR("Sorry, no rendersystem was found.");
            return;
        }

        Ogre::RenderSystem *renderSystem = renderSystemList.at(0);
        mRoot->setRenderSystem(renderSystem);

        mRoot->initialise(false, "", "");

        Ogre::NameValuePairList params;
        params["FSAA"] = "0"; 
        params["vsync"] = "true";
        mWindow = mRoot->createRenderWindow("ENGINE", 800, 600, false, &params);

        mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "SceneMgr");
        mRootSceneNode = mSceneMgr->getRootSceneNode();
            
        mWindow->setActive(true);
        mWindow->setAutoUpdated(false);

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
        /*
        Ogre::ResourceGroupManager& recourceMgr = Ogre::ResourceGroupManager::getSingleton();
        Ogre::String recourceGroup = "recources";
		recourceMgr.createResourceGroup(recourceGroup);
        recourceMgr.addResourceLocation("Models", "FileSystem", recourceGroup, false);
        recourceMgr.initialiseResourceGroup(recourceGroup);
        recourceMgr.loadResourceGroup(recourceGroup);
        */
        
        mSceneMgr->setAmbientLight(Ogre::ColourValue(0,0,0));
        mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
        mSceneMgr->setShadowFarDistance(500);

        
        mRoot->clearEventTimes();

    }catch(Ogre::Exception &e)
    {
        ENGINE_ERROR(e.what());
    }catch(std::exception &e)
    {
        ENGINE_ERROR(e.what());
    }catch(...)
    {
        ENGINE_ERROR("Unknown Error");
    }
}

SystemGraphics::~SystemGraphics()
{
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
}

void SystemGraphics::init()
{
}
    
void SystemGraphics::update(float elapsedTime)
{
    mWindow->update(false);
 
    mWindow->swapBuffers();
 
    mRoot->renderOneFrame();
 
    Ogre::WindowEventUtilities::messagePump();
}

void SystemGraphics::receiveMessage(Message *msg)
{
}
