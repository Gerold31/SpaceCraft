#include "Bot.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreEntity.h"

#include "ENGINE.hpp"
#include "SpaceShipPart.hpp"

#define WAYPOINT_THRESHOLD (1.0)

std::vector<Bot::Task *>Bot::mTaskPool;

Bot::Bot(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Human(pos, ori, parent, name, engine)
{
    mTask = NULL;
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
			mPath = Pathfinding::findPath(NULL, getStandOn(), (SpaceShipPart *)mTask->mTarget);
			break;
		case Bot::Task::TASK_TYPE_KILL:
			mPath = Pathfinding::findPath(NULL, getStandOn(), ((Human *)mTask->mTarget)->getStandOn());
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
		
		if(mPath->mWaypoints->size() == 1) // change to "is in sight"
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
