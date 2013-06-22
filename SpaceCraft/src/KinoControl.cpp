#include "KinoControl.hpp"

#include "ENGINE.hpp"
#include "Kino.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreHardwarePixelBuffer.h"
#include "OGRE/OgreMaterial.h"
#include "OGRE/OgreSubEntity.h"


KinoControl::KinoControl(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, "SC_KinoControl", engine)
{
    mEntity = engine->getSceneMgr()->createEntity(name + "Ent", "KinoControl.mesh");
    mNode->attachObject(mEntity);

    mKino = new Kino(pos + Ogre::Vector3(0, 5, 0), ori, parent, name + "_Kino", engine);

    Ogre::TextureManager& textureManager = Ogre::TextureManager::getSingleton();
	Ogre::String textureName = name + "Texture";

    Ogre::TexturePtr textureWithRtt = textureManager.createManual(textureName, "recources", 
		Ogre::TEX_TYPE_2D, 1024, 1024, 0,
		Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET, 0, false, 0);
    Ogre::RenderTexture* renderTarget = NULL;
	
	Ogre::HardwarePixelBufferSharedPtr rttBuffer = textureWithRtt->getBuffer();
	renderTarget = rttBuffer->getRenderTarget();
	renderTarget->setAutoUpdated(true);
    
    Ogre::Viewport *rttViewport1 = renderTarget->addViewport(mKino->getCamera(), 50, 0.00f, 0.00f, 1.0f, 1.0f);
	rttViewport1->setAutoUpdated(true);
    rttViewport1->setBackgroundColour(Ogre::ColourValue(1,.5,.8));
    	
    Ogre::MaterialManager& materialManager = Ogre::MaterialManager::getSingleton();
    Ogre::MaterialPtr material = materialManager.create(name + "Material", "recources");
	Ogre::Technique *technique = material->getTechnique(0);
	Ogre::Pass *pass = technique->getPass(0);
	Ogre::TextureUnitState* textureUnit = pass->createTextureUnitState();
	textureUnit->setTextureName(textureName);
    textureUnit->setTextureScale(-1, -1);
    mEntity->getSubEntity("Screen")->setMaterial(material);
    /*
    textureUnit->setNumMipmaps(0);
	textureUnit->setTextureFiltering(Ogre::TFO_BILINEAR);
    */
}

bool KinoControl::update(float elapsedTime)
{
    return true;
}