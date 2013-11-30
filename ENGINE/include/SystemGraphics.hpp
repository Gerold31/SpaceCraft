#ifndef _SYSTEMGRAPHICS_HPP_
#define _SYSTEMGRAPHICS_HPP_

#include "System.hpp"

namespace Ogre
{
    class RenderWindow;
    class RenderSystem;
    class Root;
    class SceneManager;
    class SceneNode;
}

namespace ENGINE
{

class SystemGraphics : public System, public Singleton<SystemGraphics>
{
friend class Singleton<SystemGraphics>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);
    
    Ogre::RenderWindow  *getWindow()    {return mWindow;}
    Ogre::Root          *getRoot()      {return mRoot;}
    Ogre::SceneManager  *getSceneMgr()  {return mSceneMgr;}

private:
    SystemGraphics();
    SystemGraphics(const SystemGraphics &) : System("SystemGraphics") {}
    ~SystemGraphics();

    Ogre::Root          *mRoot;
    Ogre::RenderWindow  *mWindow;
    Ogre::SceneManager  *mSceneMgr;
    Ogre::SceneNode     *mRootSceneNode;

};

};

#endif
