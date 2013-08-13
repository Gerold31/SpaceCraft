#include "SpaceShipPart.hpp"

#include "ENGINE.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreSubEntity.h"

SpaceShipPart::SpaceShipPartInfo::SpaceShipPartInfo(PART_TYPE partType, Ogre::Vector3 pos, Ogre::Quaternion rot, bool placable)
{
    mPartType = partType;
    mPos = pos;
    mRot = rot;
    mPlacable = placable;
}

SpaceShipPart::SpaceShipPart(PART_TYPE partType, bool castShadows, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, Ogre::String type, ENGINE *engine)
    :Entity(pos, ori, parent, name, type, engine)
{
    mPartType = partType;
    mEntity = NULL;
}

SpaceShipPart::SpaceShipPart(PART_TYPE partType, bool castShadows, Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::StaticGeometry *staticGeometry, Ogre::String name, Ogre::String type, ENGINE *engine)
    :Entity(pos, ori, parent, name, type, engine)
{
    mPartType = partType;
    mEntity = NULL;
    mStaticGeometry = staticGeometry;
}

SpaceShipPart::SpaceShipPart(SpaceShipPart *old, Ogre::String name)
    :Entity(old->getParentSceneNode()->getPosition(), old->getParentSceneNode()->getOrientation(), old->getParentSceneNode()->getParentSceneNode(), name, old->mType, old->mEngine)
{
    mPartType = old->mPartType;
    mNeighbor = old->mNeighbor;
    mEntity = old->mEntity->clone(name + "Ent");
    mEntity->setMaterialName("Part/Metal");
    mEntity->setCastShadows(true);
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
}

SpaceShipPart::SpaceShipPart(SpaceShipPart *old, Ogre::String name, Ogre::String type)
    :Entity(old->getParentSceneNode()->getPosition(), old->getParentSceneNode()->getOrientation(), old->getParentSceneNode()->getParentSceneNode(), name, type, old->mEngine)
{
    mPartType = old->mPartType;
    mNeighbor = old->mNeighbor;
    mEntity = old->mEntity->clone(name + "Ent");
    mEntity->setMaterialName("Part/Metal");
    mEntity->setCastShadows(true);
    mEntity->getUserObjectBindings().setUserAny("Entity", Ogre::Any((Entity *)this));
    mNode->attachObject(mEntity);
}

SpaceShipPart::~SpaceShipPart()
{
    if(mEntity)
        mEngine->getSceneMgr()->destroyEntity(mEntity);
    mEntity = NULL;
}

void SpaceShipPart::setMaterial(Ogre::String name)
{
    if(mEntity)
        mEntity->setMaterialName(name);
}
/*
void SpaceShipPart::setupInstancedMaterialToEntity(Ogre::Entity *ent)
{
	for(Ogre::uint i=0; i<ent->getNumSubEntities(); ++i)
	{
		Ogre::SubEntity* se = ent->getSubEntity(i);
		Ogre::String materialName= se->getMaterialName();
        se->setMaterialName(buildMaterial(materialName));
	}
}

Ogre::String SpaceShipPart::buildMaterial(const Ogre::String &originalMaterialName)
{
	// already instanced ?
	if(Ogre::StringUtil::endsWith (originalMaterialName, "/instanced"))
		return originalMaterialName;

	Ogre::MaterialPtr originalMaterial = Ogre::MaterialManager::getSingleton ().getByName (originalMaterialName);

	// if originalMat doesn't exists use "Instancing" material name
	const Ogre::String instancedMaterialName (originalMaterial.isNull() ? "Instancing" : originalMaterialName + "/Instanced");
	Ogre::MaterialPtr  instancedMaterial = Ogre::MaterialManager::getSingleton ().getByName (instancedMaterialName);

	// already exists ?
	if(instancedMaterial.isNull())
	{
		instancedMaterial = originalMaterial->clone(instancedMaterialName);
		instancedMaterial->load();
		Ogre::Technique::PassIterator pIt = instancedMaterial->getBestTechnique ()->getPassIterator();
		while(pIt.hasMoreElements())
		{

			Ogre::Pass * const p = pIt.getNext();
			p->setVertexProgram("Crowd",true);
			p->setShadowCasterVertexProgram("CrowdShadowCaster");
		
		}
	}
	instancedMaterial->load();
	return instancedMaterialName;


}
*/
