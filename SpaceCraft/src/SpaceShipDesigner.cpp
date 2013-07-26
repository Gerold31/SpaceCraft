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
	//create a raycast straight out from the camera at the mouse's location
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(e.state.X.abs/float(e.state.width), e.state.Y.abs/float(e.state.height));
	mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator i = result.begin();
 
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

                            removePossiblePart(part);
                            
                            debugShip(SpaceShipPart::PART_FLOOR);
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
    for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
    {
        SpaceShipPart *part = mSpaceShip->getPart(i);
        addPossibleParts(part);
    }
    debugShip(SpaceShipPart::PART_FLOOR);
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
    for(size_t i=0; i<part->getNumberNeighbors(); i++)
    {
        SpaceShipPart *neighbor = part->getNeighbor(i);
        if(neighbor == NULL)
        {
            //printf("\tneighbor %d is null\n", i);
            SpaceShipPart::SpaceShipPartInfo *info = part->getNeighborInfo(i);
            Ogre::Vector3 pos = part->getSceneNode()->getPosition() + info->mPos;
            Ogre::Quaternion rot = part->getSceneNode()->getOrientation() * info->mRot;
            SpaceShipPart *other;
            bool found = false;
            for(std::vector<SpaceShipPart *>::iterator j = mPossibleParts.begin(); j != mPossibleParts.end(); ++j)
            {
                other = *j;
                if(other && other->getPartType() == info->mPartType && other->getSceneNode()->getPosition() == pos)
                {
                    found = true;
                    //printf("\t\tpart %s is new neighbor\n", other->getName().c_str());
                    break;
                }
            }
            if(!found)
            {
                char name[32];
                sprintf(name, "DesignerPart%d", mNextPartID);
                mNextPartID++;
                other = new SpaceShipPart(info->mPartType, pos, rot, mSpaceShip->getSceneNode(), name, mEngine);
                if(info->mPartType != mSelectedPartType)
                    other->getSceneNode()->setVisible(false);
                other->setMaterial("DesignerPart");
                //printf("\t\tnew part %s is new neighbor\n", other->getName().c_str());
                mPossibleParts.push_back(other);
            }
            part->setNeighbor(other, i);
            int id = info->mOtherID;
            if(id == -1)
            {
                for(size_t k = 0; k<other->getNumberNeighbors(); k++)
                {
                    SpaceShipPart::SpaceShipPartInfo *nInfo = other->getNeighborInfo(k);
                    if(nInfo->mPartType == part->getPartType() && pos + nInfo->mPos == part->getSceneNode()->getPosition())
                    {
                        id = k;
                        break;
                    }
                }
            }
            other->setNeighbor(part, id);
        }
    }
}

void SpaceShipDesigner::removePossiblePart(SpaceShipPart *part)
{
    for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
    {
        if(*i == part)
        {
            mEngine->getMap()->destroyEntity(*i);
            mPossibleParts.erase(i);
            return;
        }
    }
    printf("No Part deleted!\n");
}

void SpaceShipDesigner::debugShip(int partType)
{
    printf("==========\nShipdump:\n");
    printf("\tShipParts:\n");
    for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
    {
        SpaceShipPart *part = mSpaceShip->getPart(i);
        if(partType == -1 || partType == part->getPartType())
            debugPart(part, partType);
    }
    printf("\tDesignerParts:\n");
    for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
    {
        SpaceShipPart *part = *i;
        if(partType == -1 || partType == part->getPartType())
            debugPart(part, partType);
    }
    printf("==========\n");
}

void SpaceShipDesigner::debugPart(SpaceShipPart *part, int partType)
{
    printf("\t\tName: %s\n", part->getName().c_str());
    printf("\t\tPos:  %s\n", Ogre::StringConverter::toString(part->getSceneNode()->getPosition()).c_str());
    for(size_t i=0; i<part->getNumberNeighbors(); i++)
    {
        SpaceShipPart *neighbor = part->getNeighbor(i);
        if(neighbor)
        {
            if(partType == -1 || partType == neighbor->getPartType())
            {
                printf("\t\t\tNeighbor %d:\n", i);
                printf("\t\t\t\tName: %s\n", neighbor->getName().c_str());
                printf("\t\t\t\tPos:  %s\n", Ogre::StringConverter::toString(neighbor->getSceneNode()->getPosition()).c_str());
            }
        }
    }
}