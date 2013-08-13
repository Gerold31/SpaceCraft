#ifndef _FLASHLIGHT_HPP_
#define _FLASHLIGHT_HPP_

#include "Entity.hpp"

class Flashlight : public Entity
{
public:
    Flashlight(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    ~Flashlight();

    bool update(float elapsedTime);
    
    void toggle();
    void toggle(bool toggle);

    static std::string getType(){return mType;}

private:
    static std::string mType;

    Ogre::Entity *mEntity;
    Ogre::Light *mLight;
};

#endif
