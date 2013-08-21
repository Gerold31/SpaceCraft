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

    mDelay = 15.0;
    
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
    if((mDelay -= elapsedTime) > 0)
        return true;
    mDelay = 0;
    if(mTaskPool.size() > 0 && (!mTask || !!mPath))
    {
        Task *task = mTaskPool.at(rand()%mTaskPool.size());
        Pathfinding::Path *path = task->getPath(this);
        if(!task->mAssignedTo && path)
        {
            if(mTask)
                mTask->mAssignedTo = NULL;
            mTask = task;
            mPath = path;
            mTask->mAssignedTo = this;
        }
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
            float speed = 1.0;
            Ogre::Vector3 dir = mPath->mWaypoints->at(MIN(1, mPath->mWaypoints->size() - 1))->getParentSceneNode()->_getDerivedPosition();
            dir.y = pos.y;
            dir = dir - pos;
            Ogre::Ray ray(pos + Ogre::Vector3(0, -0.5, 0), dir.normalisedCopy());
            mRaySceneQuery->setRay(ray);
            mRaySceneQuery->setSortByDistance(true);

            Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
            Ogre::RaySceneQueryResult::iterator i = result.begin();

            bool collision = false;
            bool blocked = false;
            while(i!=result.end())
            {
                if(i->distance > dir.length())
                    break;
                if(i->movable && i->movable->getMovableType() == "Entity" && i->movable->getName() != mNode->getName() + "Mesh")
                {
                    Entity *ent = Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
                    if(ent->getType() == SpaceShipPartDoor::getType())
                    {
                        SpaceShipPartDoor *door = (SpaceShipPartDoor *)ent;
                        if(!door->isOpen())
                        {
                            if(door->isLocked())
                            {
                                // @todo shoot
                                if(i->distance < 1.5)
                                {
                                    blocked = true;
                                }
                            }else
                            {
                                if(i->distance < 1.5)
                                {
                                    mDelay = 0.5;
                                    door->open(true);
                                }
                            }
                        }
                    }
                    if(ent->getType() == Bot::getType())
                    {
                        if((pos + dir).distance(ent->getParentSceneNode()->getPosition()) > dir.length())
                            speed *= 1.1;
                        else
                            speed /= 1.1;
                    }
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
            if(!blocked)
            {
                mNode->translate((target - pos).normalisedCopy() * elapsedTime * speed);
            }
            mNode->lookAt(target, Ogre::Node::TS_WORLD);
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

Pathfinding::Path *Bot::Task::getPath(Bot *bot)
{
    switch (mType)
    {
    case Bot::Task::TASK_TYPE_DESTROY:
        return Pathfinding::findHumanPath(NULL, bot->getStandOn(), ((SpaceShipPart *)mTarget)->getNeighbor(0));
    case Bot::Task::TASK_TYPE_KILL:
        return Pathfinding::findHumanPath(NULL, bot->getStandOn(), ((Human *)mTarget)->getStandOn());
    default:
        return NULL;
    }
}