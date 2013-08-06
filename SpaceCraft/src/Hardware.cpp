#include "Hardware.hpp"

Hardware::Hardware(int ID, int MF, int VER, SpaceShipPart::PART_TYPE part, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine)
    :SpaceShipPart(part, true, pos, ori, parent, name, type, engine),
    mID(ID),
    mMF(MF),
    mVER(VER),
    mCPU(NULL)
{
}