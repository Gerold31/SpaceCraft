#include "MessageObjectFactory.hpp"

#include "SystemGraphics.hpp"

using namespace ENGINE;

int MessageCreateObject::mID = -1;

void *MessageCreateObject::CreateMessage(std::istream &stream) 
{
    Ogre::String name; std::string type;
    std::string px, py, pz, ow, ox, oy, oz, parentName;

    stream >> px >> py >> pz >> ow >> ox >> oy >> oz >> parentName >> name >> type;

    std::cout << "px: " << px << "\npy: " << py << "\npz: " << pz << "\now: " << ow << "\nox: " << ox << "\noy: " << oy << "\noz: " << oz << "\nparentName: " << parentName << "\nname: " << name << "\ntype: " << type << std::endl;

    Ogre::Vector3 pos(atof(px.c_str()), atof(py.c_str()), atof(pz.c_str()));
    Ogre::Quaternion ori(atof(ow.c_str()), atof(ox.c_str()), atof(oy.c_str()), atof(oz.c_str()));

    Ogre::SceneNode *parent;
    if(parentName == SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode()->getName().c_str())
        parent = SystemGraphics::getSingleton()->getSceneMgr()->getRootSceneNode();
    else
        parent = SystemGraphics::getSingleton()->getSceneMgr()->getSceneNode(parentName);

    return new MessageCreateObject(pos, ori, parent, name, type); 
}

void MessageCreateObject::_serialize(std::ostream &stream) 
{
    stream << mPos.x << " " << mPos.y << " " << mPos.z << " " << mOri.w << " " << mOri.x << " " << mOri.y << " " << mOri.z << " " << mParent->getName().c_str() << " " << mName << " " << mType << " ";
    std::cout << "px: " << mPos.x << "\npy: " << mPos.y << "\npz: " << mPos.z << "\now: " << mOri.w << "\nox: " << mOri.x << "\noy: " << mOri.y << "\noz: " << mOri.z << "\nparentName: " << mParent->getName().c_str() << "\nname: " << mName << "\ntype: " << mType << std::endl;
}