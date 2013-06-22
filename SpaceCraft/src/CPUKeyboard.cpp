#include "CPUKeyboard.hpp"

#include "ENGINE.hpp"
#include "CPU.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"

CPUKeyboard::CPUKeyboard(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    :Hardware(0x7349F615, 0x1C6C8B36, 0x1802, pos, ori, parent, name, "CPU_Keyboard", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Mesh", "CPUKeyboard.mesh");
    mNode->attachObject(mEntity);
}

bool CPUKeyboard::update(float elapsedTime)
{
    return true;
}

void CPUKeyboard::interrupt()
{
    switch(mCPU->getRegister(0))
    {
    case 0: // MEM_MAP_SCREEN
    case 1: // MEM_MAP_FONT
    case 2: // MEM_NAP_PALETTE
    case 3: // SET_BORDER_COLOR
    case 4: // MEM_DUMP_FONT
    case 5: // MEM_DUMP_PALETTE
        break;
    default:
        break;
    }
}