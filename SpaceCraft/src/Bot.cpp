#include "Bot.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreEntity.h"

#include "ENGINE.hpp"
#include "SpaceShipPart.hpp"
#include "SpaceShipPartDoor.hpp"
#include "Flashlight.hpp"
#include "Weapon.hpp"

#define WAYPOINT_THRESHOLD (0.2)

#define MIN(x, y) (x < y ? x : y)

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
        Ogre::Vector3 target = mPath->mWaypoints->front()->getParentSceneNode()->_getDerivedPosition();
        Ogre::Vector3 pos = mNode->_getDerivedPosition();
        target.y = pos.y;
        if(mPath->mWaypoints->size() > 1)
        {
            if(target.distance(pos) < WAYPOINT_THRESHOLD)
            {
                mPath->mWaypoints->erase(mPath->mWaypoints->begin());
            }
        }

        bool isInSight = false;

        Ogre::Ray ray(pos + Ogre::Vector3(0, -0.5, 0), mTask->mTarget->getParentSceneNode()->_getDerivedPosition() - pos);
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
                // @todo shoot
                break;
            case Task::TASK_TYPE_KILL:
                // @todo shoot (& move?)
                break;
            }
        }else
        {
            //collision detection
            Ogre::Vector3 dir = mPath->mWaypoints->at(MIN(1, mPath->mWaypoints->size() - 1))->getParentSceneNode()->_getDerivedPosition();
            dir.y = pos.y;
            dir = dir - pos;
            Ogre::Ray ray(pos + Ogre::Vector3(0, -0.5, 0), dir.normalisedCopy());
            mRaySceneQuery->setRay(ray);
            mRaySceneQuery->setSortByDistance(true);

            Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
            Ogre::RaySceneQueryResult::iterator i = result.begin();

            bool collision = false;
            while(i!=result.end())
            {
                if(i->distance > dir.length())
                    break;
                if(i->movable && i->movable->getMovableType() == "Entity" && i->movable->getName() != mNode->getName() + "Mesh")
                {
                    /*
                    Entity *ent = Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
                    if(ent->getType() == SpaceShipPartDoor::getType())
                    {
                    }*/
                    collision = true;
                    break;
                }
                ++i;
            }
            if(!collision)
            {
                if(mPath->mWaypoints->size() > 1)
                    mPath->mWaypoints->erase(mPath->mWaypoints->begin());
                target = mPath->mWaypoints->front()->getParentSceneNode()->_getDerivedPosition();
                target.y = pos.y;
            }
            float speed = 1.0;
            mNode->lookAt(target, Ogre::Node::TS_WORLD);
            mNode->translate((target - pos).normalisedCopy() * elapsedTime * speed);
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
