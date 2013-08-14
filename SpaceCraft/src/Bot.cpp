#include "Bot.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreEntity.h"

#include "ENGINE.hpp"
#include "SpaceShipPart.hpp"
#include "Flashlight.hpp"
#include "Weapon.hpp"

#define WAYPOINT_THRESHOLD (1.0)

std::vector<Bot::Task *>Bot::mTaskPool;

Bot::Bot(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Human(pos, ori, parent, name, engine)
{
    mTask = NULL;
    
    mFlashlight = new Flashlight(Ogre::Vector3(-0.2, -0.15, -0.45), Ogre::Quaternion(), mNode, name + "Flashlight", engine);
    mWeapon = new Weapon(this, Ogre::Vector3(0.2, -0.15, -0.45), Ogre::Quaternion(), mNode, name + "Weapon", engine);
}

Bot::~Bot()
{
    for(int i=0; i<mTaskPool.size(); i++)
    {
        if(mTaskPool.at(i)->mAssignedTo == this)
            mTaskPool.at(i)->mAssignedTo = NULL;
    }
}

bool Bot::update(float elapsedTime)
{
    if(!mTask && mTaskPool.size() > 0)
    {
        // @todo check if task is already assigned
        // @todo first check path, then assign
        mTask = mTaskPool.at(rand()%mTaskPool.size()); 
        mTask->mAssignedTo = this;
        mPath = NULL;
    }

    if(!mPath)
    {
        switch (mTask->mType)
        {
        case Bot::Task::TASK_TYPE_DESTROY:
            mPath = Pathfinding::findHumanPath(NULL, getStandOn(), ((SpaceShipPart *)mTask->mTarget)->getNeighbor(0));
            break;
        case Bot::Task::TASK_TYPE_KILL:
            mPath = Pathfinding::findHumanPath(NULL, getStandOn(), ((Human *)mTask->mTarget)->getStandOn());
            break;
        default:
            break;
        }
        if(!mPath)
            printf("found no path\n");
    }
    
    if(mTask && mPath)
    {
        if(mPath->mWaypoints->size() == 0)
            return true;
        Ogre::Vector3 target = mPath->mWaypoints->front()->getParentSceneNode()->getPosition(); 
        target.y = mNode->getPosition().y;
        if(mPath->mWaypoints->size() > 1)
        {
            if(target.distance(mNode->getPosition()) < WAYPOINT_THRESHOLD)
            {
                mPath->mWaypoints->erase(mPath->mWaypoints->begin());
            }
        }

        bool isInSight = false;

        Ogre::Ray ray(mNode->getParentSceneNode()->getPosition() + mNode->getPosition() + Ogre::Vector3(0, -0.5, 0), mTask->mTarget->getParentNode()->getPosition() - mNode->getPosition());
        mRaySceneQuery->setRay(ray);
        mRaySceneQuery->setSortByDistance(true);

        Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = result.begin();

        while(i!=result.end())
        {
            if(i->movable && i->movable->getMovableType() == "Entity" && i->movable->getName() != mNode->getName() + "Mesh")
            {
                Entity *ent = Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
                if(ent == mTask->mTarget)
                isInSight = true;
                break;
            }
            ++i;
        }
        
        if(isInSight)
        {
            switch(mTask->mType)
            {
            case Task::TASK_TYPE_DESTROY:
                // shoot
                break;
            case Task::TASK_TYPE_KILL:
                // shoot (& move)
                break;
            }
        }else
        {
            // @todo collision detection
            float speed = 3.0;
            mNode->lookAt(target, Ogre::Node::TS_WORLD);
            mNode->translate((target - mNode->getPosition()).normalisedCopy() * elapsedTime * speed);
        }
    }
    return true;
}

void Bot::addTask(Task *task)
{
    mTaskPool.push_back(task);
}

Bot::Task::Task(TaskType type, int priority, Entity *target)
{
    mType = type;
    mPriority = priority;
    mTarget = target;
    mAssignedTo = NULL;
}
