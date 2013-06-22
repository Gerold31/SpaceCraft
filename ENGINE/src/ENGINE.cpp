#include "ENGINE.hpp"

#include "Map.hpp"

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreWindowEventUtilities.h"

#include <vector>
#include <exception>
#include <stdio.h>

#define ENGINE_ERROR(x) printf(x);


ENGINE::ENGINE()
{
    mRoot = NULL;
    mWindow = NULL;
    mMap = NULL;
}

ENGINE::~ENGINE()
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

bool ENGINE::init(Map *map)
{
    try{
        mRoot = new Ogre::Root("", "", "Ogre.log");

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

    
        const Ogre::RenderSystemList& renderSystemList = mRoot->getAvailableRenderers();
        if( renderSystemList.size() == 0 )
        {
            ENGINE_ERROR("Sorry, no rendersystem was found.");
            return false;
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

        Ogre::ResourceGroupManager& recourceMgr = Ogre::ResourceGroupManager::getSingleton();
        Ogre::String recourceGroup = "recources";
		recourceMgr.createResourceGroup(recourceGroup);
        recourceMgr.addResourceLocation("Models", "FileSystem", recourceGroup, false);
        recourceMgr.initialiseResourceGroup(recourceGroup);
        recourceMgr.loadResourceGroup(recourceGroup);

        mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

        if(!map)
            mMap = new Map(this);
        else
            mMap = map;
        mMap->init();

        return true;
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
    return false;
}

void ENGINE::run()
{
    mRoot->clearEventTimes();

    Ogre::Timer* timer = mRoot->getTimer();
	timer->reset();
	unsigned long lastTime = timer->getMilliseconds();
	float elapsedTime = 0;

    while(!mWindow->isClosed())
    {
        unsigned long currentTime = timer->getMilliseconds();
		elapsedTime = (currentTime - lastTime) * 1e-3;
		if(elapsedTime == 0)
			continue;
		lastTime = currentTime;

        if(!update(elapsedTime))
            break;

        mWindow->update(false);
 
        mWindow->swapBuffers(true);
 
        mRoot->renderOneFrame();
 
        Ogre::WindowEventUtilities::messagePump();
    }
}


bool ENGINE::update(float elapsedTime)
{
    return mMap->update(elapsedTime);
}
