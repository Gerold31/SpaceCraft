#include "Universe.hpp"

#include "ENGINE.hpp"
#include "Star.hpp"
#include "GravityObject.hpp"

#include "OGRE/OgreSceneManager.h"

#define G (6.67384e-11)

Universe::Universe(ENGINE *engine)
    : Map(engine)
{
}

void Universe::init()
{
}

bool Universe::update(float elapsedTime)
{
    std::vector<GravityObject *>::iterator i = mGravityObjects.begin();
    for(;i!=mGravityObjects.end(); ++i)
    {
        GravityObject *g1 = *i, *g2;
        Ogre::Vector3 pos1 = g1->getParentSceneNode()->getPosition(), pos2;
        Ogre::Vector3 netForce = Ogre::Vector3::ZERO;

        std::vector<GravityObject *>::iterator j = mGravityObjects.begin();
        for(;j!=mGravityObjects.end(); ++j)
        {
            if(i==j)
                continue;
            
            g2 = (*j);
            pos2 = g2->getParentSceneNode()->getPosition();
            
            double r = pos1.distance(pos2);
            double f = G * g1->getMass() * g2->getMass() / (r * r);
            netForce += (pos1 - pos2) * f / r;
        }
        g1->applyForce(netForce, elapsedTime);
    }

    return Map::update(elapsedTime);
}
