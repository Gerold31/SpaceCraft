#include "SpaceShipDesigner.hpp"

#include "ENGINE.hpp"
#include "Map.hpp"
#include "GUIManager.hpp"
#include "SpaceShip.hpp"
#include "SpaceShipPart.hpp"
#include "SpaceShipDesignerGUI.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRay.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreStringConverter.h"

SpaceShipDesigner::SpaceShipDesigner(ENGINE *engine)
{
    mEngine = engine;
    mNode = mEngine->getSceneMgr()->getRootSceneNode()->createChildSceneNode("SpaceShipDesignerNode");
    mNode->setPosition(Ogre::Vector3(0,10,0));
    mNode->setOrientation(Ogre::Quaternion::IDENTITY);

    mCamera = mEngine->getSceneMgr()->createCamera("SpaceShipDesignerCamera");
    mCamera->lookAt(0, 0, -10);
    mCamera->setNearClipDistance(0.5);
    mCamera->setFarClipDistance(5000.0);

    mCameraYawNode   = mNode->createChildSceneNode();
    mCameraPitchNode = mCameraYawNode->createChildSceneNode();
    mCameraRollNode  = mCameraPitchNode->createChildSceneNode();
    mCameraRollNode->attachObject(mCamera);
    mCameraPitchNode->pitch(Ogre::Radian(-Ogre::Math::PI/2));
    
    mRaySceneQuery = engine->getSceneMgr()->createRayQuery(Ogre::Ray());

    mGUI = new SpaceShipDesignerGUI(engine, this);
    mGUI->setVisible(false);

    mSelectedPartType = -1;
}

void SpaceShipDesigner::enterEditMode(SpaceShip *ship) 
{
    mSpaceShip = ship;
    mViewport = mEngine->getWindow()->addViewport(mCamera, 100, 0, 0, 1, 1);
    
    mViewport->setAutoUpdated(true);
    mViewport->setBackgroundColour(Ogre::ColourValue(0,0,.6));
    mCamera->setAspectRatio(1.0f * mViewport->getActualWidth() / mViewport->getActualHeight());

    mGUI->setVisible(true);
    
    mSelectedPartType = -1;
    mNextPartID = 0;

    initPossibleParts();
    //mNode->setPosition(mSpaceShip->getSceneNode()->getPosition() + Ogre::Vector3(0, 10, 0));
}

void SpaceShipDesigner::exitEditMode()
{
    mEngine->getWindow()->removeViewport(100);

    removePossibleParts();

    mGUI->setVisible(false);
}

bool SpaceShipDesigner::keyPressed(const OIS::KeyEvent &e)
{
    return true;
}

bool SpaceShipDesigner::keyReleased(const OIS::KeyEvent &e)
{
    return true;
} 

bool SpaceShipDesigner::mouseMoved(const OIS::MouseEvent &e)
{
    mNode->setPosition(mNode->getPosition() + Ogre::Vector3(0, -e.state.Z.rel/240.0, 0));
    return true;
}

bool SpaceShipDesigner::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    printf("click\n");
    //find the current mouse position
    MyGUI::IntPoint pos = mEngine->getGUIManager()->getMousePos();
 
	//create a raycast straight out from the camera at the mouse's location
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(e.state.X.abs/float(e.state.width), e.state.Y.abs/float(e.state.height));
	mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator i = result.begin();
 
	//check to see if the mouse is pointing at the world and put our current object at that location
    while(i != result.end())
	{
        if(i->movable && i->movable->getMovableType() == "Entity")
        {
            Ogre::SceneNode *node = i->movable->getParentSceneNode();
            if(node)
            {
                Entity *obj = dynamic_cast<Entity *>(node->getAttachedObject(node->getName() + "Obj"));
                if(obj)
                {              
                    if(obj->getType() == "SC_SpaceShipPart")
                    {
                        SpaceShipPart *part = (SpaceShipPart *)obj;
                        if(part->getPartType() == mSelectedPartType && part->getName().find("Designer") == 0)
                        {
                            char name[32];
                            sprintf(name, "%sPart%d", mSpaceShip->getSceneNode()->getName().c_str(), mSpaceShip->getNextPartID());
                            SpaceShipPart *newPart = new SpaceShipPart(part, name);
                            printf("add SpaceShipPart %s at %s\n", newPart->getSceneNode()->getName().c_str(), Ogre::StringConverter::toString(newPart->getSceneNode()->getPosition()).c_str());
                            for(size_t j=0; j<newPart->getNumberNeighbors(); j++)
                            {
                                SpaceShipPart *neighbor = newPart->getNeighbor(j);
                                if(neighbor)
                                {
                                    for(size_t k=0; k<neighbor->getNumberNeighbors(); k++)
                                    {
                                        if(neighbor->getNeighbor(k) == part)
                                        {
                                            neighbor->setNeighbor(newPart, k);
                                            break;
                                        }
                                    }                                    
                                }
                            }
                            mSpaceShip->addPart(newPart);
                            for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
                            {
                                if(*i == part)
                                {
                                    mPossibleParts.erase(i);
                                }
                            }
                            
                            printf("remove DesignerPart %s at %s\n", part->getName().c_str(), Ogre::StringConverter::toString(part->getSceneNode()->getPosition()).c_str());
                            mEngine->getMap()->destroyEntity(part);
                            
                            addPossibleParts(newPart);
                            break;
                        }
                    }
                }
            }
        }
        ++i;
	}	

    return true;
}

bool SpaceShipDesigner::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

void SpaceShipDesigner::setSelectedPartType(int type)
{
    if(mSelectedPartType != type)
    {
        mSelectedPartType = type;
        updateSelectedPartType();
    }
}

void SpaceShipDesigner::initPossibleParts()
{
    std::vector<SpaceShipPart *>parts;
    for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
    {
        parts.push_back(mSpaceShip->getPart(i));
    }
    while(parts.size() > 0)
    {
        SpaceShipPart *cur = parts.back();
        parts.pop_back();
        addPossibleParts(cur);
    }
}

void SpaceShipDesigner::removePossibleParts()
{
    for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
    {
        SpaceShipPart *part = mSpaceShip->getPart(i);
        for(size_t j=0; j<part->getNumberNeighbors(); j++)
        {
            if(part->getNeighbor(j) && part->getNeighbor(j)->getName().find("Designer") == 0)
                part->setNeighbor(NULL, j);
        }
    }
    while(mPossibleParts.size() > 0)
    {
        mEngine->getMap()->destroyEntity(mPossibleParts.back());
        mPossibleParts.pop_back();
    }
}

void SpaceShipDesigner::updateSelectedPartType()
{
    for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
    {
        if((*i)->getPartType() == mSelectedPartType)
        {
            (*i)->getSceneNode()->setVisible(true);
        }else
        {
            (*i)->getSceneNode()->setVisible(false);
        }
    }
}

void SpaceShipDesigner::addPossibleParts(SpaceShipPart *part)
{
    printf("addPossibleParts from %s\n", part->getName().c_str());
    for(size_t i=0; i<part->getNumberNeighbors(); i++)
    {
        SpaceShipPart *neighbor = part->getNeighbor(i);
        if(neighbor == NULL)
        {
            printf("\tneighbor %d is empty\n", i);
            SpaceShipPart::SpaceShipPartInfo *info = part->getNeighborInfo(i);
            Ogre::Vector3 pos = part->getSceneNode()->getPosition() + info->mPos;
            bool found = false;
            for(std::vector<SpaceShipPart *>::iterator j = mPossibleParts.begin(); j != mPossibleParts.end(); ++j)
            {
                SpaceShipPart *possible = *j;
                if(possible->getPartType() == info->mPartType && possible->getSceneNode()->getPosition() == pos)
                {
                    printf("\t\tpossible part %s needs to fill this slot\n", possible->getName());
                    part->setNeighbor(possible, i);
                    int id = info->mOtherID;
                    if(id == -1)
                    {
                        for(size_t k = 0; k<possible->getNumberNeighbors(); k++)
                        {
                            SpaceShipPart::SpaceShipPartInfo *nInfo = possible->getNeighborInfo(k);
                            if(nInfo->mPartType == part->getPartType() && possible->getSceneNode()->getPosition() + nInfo->mPos == part->getSceneNode()->getPosition())
                            {
                                id = k;
                                break;
                            }
                        }
                    }
                    printf("\t\tother id is %d\n", id);
                    possible->setNeighbor(part, id);
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                printf("\t\tneed to create new part\n");
                char name[32];
                sprintf(name, "DesignerPart%d", mNextPartID);
                mNextPartID++;
                SpaceShipPart *newPart = new SpaceShipPart(info->mPartType, pos, Ogre::Quaternion(), mSpaceShip->getSceneNode(), name, mEngine);
                printf("\t\tadd DesignerPart %s at %s\n", newPart->getSceneNode()->getName().c_str(), Ogre::StringConverter::toString(pos).c_str());
                printf("\t\tpart pos: %s\n", Ogre::StringConverter::toString(newPart->getSceneNode()->getPosition()).c_str());
                if(info->mPartType != mSelectedPartType)
                    newPart->getSceneNode()->setVisible(false);
                newPart->setMaterial("DesignerPart");
                part->setNeighbor(newPart, i);
                int id = info->mOtherID;
                if(id == -1)
                {
                    printf("\t\tid == -1\n");
                    for(size_t k = 0; k<newPart->getNumberNeighbors(); k++)
                    {
                        SpaceShipPart::SpaceShipPartInfo *nInfo = newPart->getNeighborInfo(k);
                        if(nInfo->mPartType == part->getPartType() && pos + nInfo->mPos == part->getSceneNode()->getPosition())
                        {
                            id = k;
                            break;
                        }
                    }
                }
                printf("\t\tother id is %d\n", id);
                newPart->setNeighbor(part, id);
                mPossibleParts.push_back(newPart);
            }
        }
    }
}