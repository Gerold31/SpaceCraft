#ifndef _BOT_HPP_
#define _BOT_HPP_

#include "Pathfinding.hpp"
#include "Human.hpp"

class Flashlight;
class Weapon;

class Bot : public Human
{
public:
    struct Task
    {
        enum TaskType
        {
            TASK_TYPE_DESTROY,
            TASK_TYPE_KILL
        };

        Task(TaskType type, int priority, Entity *target);

        TaskType mType;
        int mPriority;
        Entity *mTarget;

        Bot *mAssignedTo;
    };

    Bot(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine);
    ~Bot();

    bool update(float elapsedTime);

    static void addTask(Task *task);

protected:
    static std::vector<Task *>mTaskPool;
    Task *mTask;
	Pathfinding::Path *mPath;

    Flashlight *mFlashlight;
    Weapon *mWeapon;
};

#endif
