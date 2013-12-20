#ifndef _SYSTEMGAMESTATE_HPP_
#define _SYSTEMGAMESTATE_HPP_

#include "System.hpp"

#include "Defines.hpp"

#include "OGRE/OgreString.h"
#include "OGRE/OgreSceneNode.h"

namespace ENGINE
{

class ComponentServerConnection;
class Object;

class SystemGameState : public System, public Singleton<SystemGameState>
{
friend class Singleton<SystemGameState>;
public:
    void init();
    void update(float elapsedTime);
    void receiveMessage(Message *msg);

    ComponentServerConnection *newPlayer(ParamMap &params);
    void addObject(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, std::string type = "");

private:
    SystemGameState();
    SystemGameState(const SystemGameState &) : System("SystemGameState") {}
    ~SystemGameState();

    std::vector<std::pair<std::string, Object *> > mLoadedObjects;
    std::vector<ComponentServerConnection *> mConnectedPlayers;
    std::vector<ComponentServerConnection *> mNewPlayers;

    int mNextPlayerID;

    void onNewPlayer(ComponentServerConnection *connection);
};

};

#endif
