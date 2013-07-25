#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

namespace Ogre
{
    class RenderWindow;
    class RenderSystem;
    class Root;
    class SceneManager;
    class SceneNode;
}

class Map;
class GUIManager;

#include <stdlib.h>
#include <vector>

#include "MyGUI.h"

class ENGINE
{
public:
    ENGINE();
    ~ENGINE();

    bool init(Map *map = NULL);
    void initGUIManager();
    void run();
    
    Ogre::RenderWindow  *getWindow()    {return mWindow;}
    Ogre::Root          *getRoot()      {return mRoot;}
    Ogre::SceneManager  *getSceneMgr()  {return mSceneMgr;}
    Map                 *getMap()       {return mMap;}
    GUIManager          *getGUIManager(){return mGUIManager;}

    MyGUI::VectorWidgetPtr loadGUI(std::string fileName);

private:
    Ogre::Root          *mRoot;
    Ogre::RenderWindow  *mWindow;
    Ogre::SceneManager  *mSceneMgr;
    Ogre::SceneNode     *mRootSceneNode;

    Map                 *mMap;
    GUIManager          *mGUIManager;

    bool update(float elapsedTime);
};

#endif
