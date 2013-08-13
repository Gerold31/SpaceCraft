#include "SpaceShipDesigner.hpp"

#include "ENGINE.hpp"
#include "Map.hpp"
#include "GUIManager.hpp"
#include "SpaceShip.hpp"
#include "SpaceShipDesignerGUI.hpp"
#include "SpaceShipPartWall.hpp"
#include "SpaceShipPartFloor.hpp"
#include "SpaceShipPartDoor.hpp"
#include "SpaceShipPartLight.hpp"
#include "SpaceShipPartRotatingLight.hpp"
#include "SpaceShipPartWindow.hpp"
#include "CPU.hpp"
#include "CPUDisplay.hpp"
#include "CPUKeyboard.hpp"
#include "CPULightControl.hpp"
#include "CPUDoorControl.hpp"
#include "CPULifeSupport.hpp"
#include "CPULifeDetection.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRay.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreStringConverter.h"
#include "OGRE/OgreUserObjectBindings.h"
#include "OGRE/OgreAny.h"

#define LENGTH_THRESHOLD (1e-5)

SpaceShipDesigner::SpaceShipDesigner(float aspectRatio, ENGINE *engine)
{
    mEngine = engine;
    mParentNode = mEngine->getSceneMgr()->getRootSceneNode()->createChildSceneNode("SpaceShipDesignerParentNode");
    mParentNode->setOrientation(Ogre::Quaternion::IDENTITY);

    mNode = mParentNode->createChildSceneNode("SpaceShipDesignerNode");
    mNode->setPosition(Ogre::Vector3(0, 10, 0));
    mNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

    mCamera = mEngine->getSceneMgr()->createCamera("SpaceShipDesignerCamera");
    mCamera->setNearClipDistance(0.5);
    mCamera->setFarClipDistance(5000.0);
    mCamera->setAspectRatio(aspectRatio);

    mNode->attachObject(mCamera);
    
    mRaySceneQuery = engine->getSceneMgr()->createRayQuery(Ogre::Ray());

    mGUI = new SpaceShipDesignerGUI(engine, this);
    mGUI->setVisible(false);

    mSelectedPartType = -1;
    mRotation = 0;
}

void SpaceShipDesigner::enterEditMode(SpaceShip *ship) 
{
    mSpaceShip = ship;
    
    mEngine->getSceneMgr()->setAmbientLight(Ogre::ColourValue(1,1,1));

    mGUI->setVisible(true);
    
    mNextPartID = 0;

    mSelectedPartType = -1;
    mSelectedFloorFrom = mSelectedFloorTo = 0;
    mSelectedFloorFromEnabled = mSelectedFloorToEnabled = false;
    mMode = MODE_BUILD;

    mLinkFirst = mSelectedPart = NULL;

    initPossibleParts();
    //mNode->setPosition(mSpaceShip->getParentSceneNode()->getPosition() + Ogre::Vector3(0, 10, 0));
}

void SpaceShipDesigner::exitEditMode()
{
    removePossibleParts();
    
    mSelectedFloorFromEnabled = mSelectedFloorToEnabled = false;
    mMode = MODE_BUILD;
    updateVisibleParts();

    mGUI->setVisible(false);
}

bool SpaceShipDesigner::keyPressed(const OIS::KeyEvent &e)
{
    switch(e.key)
    {
    case OIS::KC_S:
        if(mSelectedPart)
        {
            mSpaceShip->removePart(mSelectedPart);
            mEngine->getMap()->destroyEntity(mSelectedPart);
            mSelectedPart = NULL;
        }
        mSpaceShip->save("Ship.ship");
        break;
    case OIS::KC_L:
        removePossibleParts();
        mSpaceShip->load("Ship.ship");
        initPossibleParts();
        break;
    case OIS::KC_C:
        removePossibleParts();
        mSpaceShip->load("");
        initPossibleParts();
        break;
    }
    return true;
}

bool SpaceShipDesigner::keyReleased(const OIS::KeyEvent &e)
{
    return true;
} 

bool SpaceShipDesigner::mouseMoved(const OIS::MouseEvent &e)
{
    mNode->setPosition(mNode->getPosition() + mNode->getOrientation() * Ogre::Vector3(0, 0, -e.state.Z.rel/240.0));
    if(e.state.buttonDown(OIS::MB_Middle))
    {
        mParentNode->rotate(Ogre::Quaternion(Ogre::Degree(-e.state.X.rel), Ogre::Vector3::UNIT_Y), Ogre::Node::TS_WORLD);
        mParentNode->rotate(Ogre::Quaternion(Ogre::Degree(-e.state.Y.rel), mParentNode->getOrientation().xAxis()), Ogre::Node::TS_WORLD);
    }
    if(e.state.buttonDown(OIS::MB_Right))
    {
        mParentNode->setPosition(mParentNode->getPosition() + mParentNode->getOrientation() * Ogre::Vector3(-e.state.X.rel/24.0, 0, -e.state.Y.rel/24.0));
    }
    mousePressed(e, OIS::MB_Left);
    return true;
}

bool SpaceShipDesigner::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Middle)
        mRotation++;

	//create a raycast straight out from the camera at the mouse's location
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(e.state.X.abs/float(e.state.width), e.state.Y.abs/float(e.state.height));
	mRaySceneQuery->setRay(mouseRay);
    mRaySceneQuery->setSortByDistance(true);
	Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator i = result.begin();

    bool hit = false;
 
    while(i != result.end())
	{
        if(i->movable && i->movable->getMovableType() == "Entity")
        {
            Entity *obj = Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
            if(obj && obj->getVisible())
            {   
                if(mMode == MODE_BUILD)
                {
                    if(obj == mSelectedPart)
                    {
                        ++i;
                        continue;
                    }
                    if(obj->getType() == "SC_SpaceShipPartFloor" || obj->getType() == "SC_SpaceShipPartWall")
                    {
                        SpaceShipPart *part = (SpaceShipPart *)obj;
                        SpaceShipPart::PART_TYPE type = part->getPartType();
                        if(mSelectedPart && part->getName().find("Designer") != 0)
                        {
                            SpaceShipPart::SpaceShipPartInfo *info = NULL;
                            int neighborId = -1;
                            Ogre::Quaternion rot = Ogre::Quaternion();
                            if(mSelectedPartType == SpaceShipPart::PART_FLOORMOUNT && type == SpaceShipPart::PART_FLOOR)
                            {
                                mRotation %= 4;
                                if(part->getNeighbor(0) == NULL)
                                {
                                    rot = Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI/2 * mRotation), Ogre::Vector3::UNIT_Y);
                                    info = part->getNeighborInfo(0);
                                    neighborId = 0;
                                }
                            }else if(mSelectedPartType == SpaceShipPart::PART_CEILMOUNT && type == SpaceShipPart::PART_FLOOR)
                            {
                                mRotation %= 4;
                                if(part->getNeighbor(1) == NULL)
                                {
                                    rot = Ogre::Quaternion(Ogre::Radian(Ogre::Math::PI/2 * mRotation), Ogre::Vector3::UNIT_Y);
                                    info = part->getNeighborInfo(1);
                                    neighborId = 1;
                                }
                            }else if(mSelectedPartType == SpaceShipPart::PART_WALLMOUNT && type == SpaceShipPart::PART_WALL)
                            {
                                mRotation %= 2;
                                if(part->getNeighbor(mRotation) == NULL)
                                {
                                    info = part->getNeighborInfo(mRotation);
                                    neighborId = mRotation;
                                }
                            }
                            if(info)
                            {
                                assert(mSelectedPart->getNumberNeighbors() == 1); // @todo deal with "big" parts
                                Ogre::Quaternion ori = part->getParentSceneNode()->getOrientation() * info->mRot;
                                Ogre::Vector3 pos = part->getParentSceneNode()->getPosition() + ori * info->mPos;
                                ori = ori * mSelectedPart->getNeighborInfo(0)->mRot.Inverse() * rot;
                                pos = pos - mSelectedPart->getNeighborInfo(0)->mRot.Inverse() * rot * mSelectedPart->getNeighborInfo(0)->mPos;
                                mSelectedPart->getParentSceneNode()->setPosition(pos);
                                mSelectedPart->getParentSceneNode()->setOrientation(ori);
                                hit = true;
                                if(e.state.buttonDown(OIS::MB_Left))
                                {
                                    part->setNeighbor(mSelectedPart, neighborId);
                                    mSelectedPart->setNeighbor(part, 0);
                                    mSelectedPart = NULL;
                                    setSelectedPartName(mSelectedPartName);
                                }
                            }
                        }else if(e.state.buttonDown(OIS::MB_Left) && type == mSelectedPartType && part->getName().find("Designer") == 0)
                        {
                            char name[32];
                            sprintf(name, "%sPart%d", mSpaceShip->getParentSceneNode()->getName().c_str(), mSpaceShip->getNextPartID());
                            SpaceShipPart *newPart = NULL;
                            switch(part->getPartType())
                            {
                            case SpaceShipPart::PART_FLOOR: 
                                newPart = new SpaceShipPartFloor(part, name);
                                break;
                            case SpaceShipPart::PART_WALL:
                                if(mSelectedPart && mSelectedPart->getType() == "SC_SpaceShipPartDoor")
                                    newPart = new SpaceShipPartDoor(part, name);
                                else if(mSelectedPart && mSelectedPart->getType() == "SC_SpaceShipPartWindow")
                                    newPart = new SpaceShipPartWindow(part, name);
                                else
                                    newPart = new SpaceShipPartWall(part, name);
                                break;
                            }
                            assert(newPart);
                            for(size_t j=0; j<newPart->getNumberNeighbors(); j++)
                            {
                                SpaceShipPart *neighbor = newPart->getNeighbor(j);
                                if(!neighbor)
                                    continue;
                                for(size_t k=0; k<neighbor->getNumberNeighbors(); k++)
                                {
                                    if(neighbor->getNeighbor(k) == part)
                                    {
                                        neighbor->setNeighbor(newPart, k);
                                        break;
                                    }                                    
                                }
                            }
                            removePossiblePart(part);

                            mSpaceShip->addPart(newPart);
                            addPossibleParts(newPart);

                            //debugShip();
                        }
                        break;
                    }
                }else if(e.state.buttonDown(OIS::MB_Left))
                {
                    if(mMode == MODE_WIRE)
                    {
                        if(mLinkFirst)
                        {
                            if(obj == mLinkFirst)
                                mLinkFirst = NULL;
                            else
                            {
                                assert(obj->getType().find("CPU_") == 0);
                                ((CPU *)mLinkFirst)->addDevice((Hardware *)obj);
                                ((Hardware *)obj)->connect((CPU *)mLinkFirst);
                            }
                        }else
                        {
                            assert(obj->getType() == "CPU");
                            mLinkFirst = (CPU *)obj;
                        }
                        updateVisibleParts();
                    }else if(mMode == MODE_WIRELIGHT)
                    {
                        if(mLinkFirst)
                        {
                            if(obj == mLinkFirst)
                                mLinkFirst = NULL;
                            else
                            {
                                assert(obj->getType() == "SC_SpaceShipPartLight");
                                ((CPULightControl *)mLinkFirst)->addLight((SpaceShipPartLight *)obj);
                                ((SpaceShipPartLight *)obj)->connect((CPULightControl *)mLinkFirst);
                            }
                        }else
                        {
                            assert(obj->getType() == "CPU_LightControl");
                            mLinkFirst = (CPULightControl *)obj;
                        }
                        updateVisibleParts();
                    }else if(mMode == MODE_WIREDOOR)
                    {
                        if(mLinkFirst)
                        {
                            if(obj == mLinkFirst)
                                mLinkFirst = NULL;
                            else
                            {
                                assert(obj->getType() == "SC_SpaceShipPartDoor");
                                ((CPUDoorControl *)mLinkFirst)->addDoor((SpaceShipPartDoor *)obj);
                                ((SpaceShipPartDoor *)obj)->connect((CPUDoorControl *)mLinkFirst);
                            }
                        }else
                        {
                            assert(obj->getType() == "CPU_DoorControl");
                            mLinkFirst = (CPUDoorControl *)obj;
                        }
                        updateVisibleParts();
                    }
                }
                break;
            }
        }
        ++i;
	}	

    if(!hit && mSelectedPart)
        mSelectedPart->getParentSceneNode()->setPosition(0, -1e38, 0);

    return true;
}

bool SpaceShipDesigner::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}

void SpaceShipDesigner::setMode(MODE mode)
{
    if(mMode != mode)
    {
        mMode = mode;
        mLinkFirst = NULL;
        updateVisibleParts();
    }
}

void SpaceShipDesigner::setSelectedPartType(int type)
{
    if(mSelectedPart)
    {
        mSpaceShip->removePart(mSelectedPart);
        mEngine->getMap()->destroyEntity(mSelectedPart);
        mSelectedPart = NULL;
    }

    if(mSelectedPartType != type)
    {
        mSelectedPartType = type;
    }
    else
    {
        mSelectedPartType = -1;
    }
    updateVisibleParts();
}

void SpaceShipDesigner::setSelectedPartName(std::string name)
{
    if(mSelectedPart)
    {
        mSpaceShip->removePart(mSelectedPart);
        mEngine->getMap()->destroyEntity(mSelectedPart);
        mSelectedPart = NULL;
    }
    
    char partName[32];
    sprintf(partName, "%sPart%d", mSpaceShip->getParentSceneNode()->getName().c_str(), mSpaceShip->getNextPartID());
    
    if(name == "CPU")
        mSelectedPart = new CPU(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "Display")
        mSelectedPart = new CPUDisplay(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "Keyboard")
        mSelectedPart = new CPUKeyboard(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "RotatingLight")
        mSelectedPart = new SpaceShipPartRotatingLight(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "Light")
        mSelectedPart = new SpaceShipPartLight(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "LightControl")
        mSelectedPart = new CPULightControl(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "Door")
        mSelectedPart = new SpaceShipPartDoor(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "DoorControl")
        mSelectedPart = new CPUDoorControl(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "LifeSupport")
        mSelectedPart = new CPULifeSupport(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "LifeDetection")
        mSelectedPart = new CPULifeDetection(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);
    else if(name == "Window")
        mSelectedPart = new SpaceShipPartWindow(Ogre::Vector3(0, -1e38, 0), Ogre::Quaternion(), mSpaceShip->getParentSceneNode(), partName, mEngine);

    if(!mSelectedPart)
        return;
    
    mSpaceShip->addPart(mSelectedPart);
    mSelectedPartType = mSelectedPart->getPartType();
    mSelectedPartName = name;
    printf("created %s, partType: %d\n", name.c_str(), mSelectedPartType);
    updateVisibleParts();
}

void SpaceShipDesigner::setSelectedFloorFrom(int from)
{
    if(mSelectedFloorFrom != from || !mSelectedFloorFromEnabled)
    {
        mSelectedFloorFrom = from;
        mSelectedFloorFromEnabled = true;
        updateVisibleParts();
    }
}

void SpaceShipDesigner::setSelectedFloorTo(int to)
{
    if(mSelectedFloorTo != to || !mSelectedFloorToEnabled)
    {
        mSelectedFloorTo = to;
        mSelectedFloorToEnabled = true;
        updateVisibleParts();
    }
}

void SpaceShipDesigner::enableSelectedFloorFrom(bool enabled)
{
    if(mSelectedFloorFromEnabled != enabled)
    {
        mSelectedFloorFromEnabled = enabled;
        updateVisibleParts();
    }
}

void SpaceShipDesigner::enableSelectedFloorTo(bool enabled)
{
    if(mSelectedFloorToEnabled != enabled)
    {
        mSelectedFloorToEnabled = enabled;
        updateVisibleParts();
    }
}

void SpaceShipDesigner::initPossibleParts()
{
    for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
    {
        SpaceShipPart *part = mSpaceShip->getPart(i);
        addPossibleParts(part);
    }
    //debugShip();
}

void SpaceShipDesigner::removePossibleParts()
{
    if(mSelectedPart)
    {
        mSpaceShip->removePart(mSelectedPart);
        mEngine->getMap()->destroyEntity(mSelectedPart);
        mSelectedPart = NULL;
    }

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

void SpaceShipDesigner::updateVisibleParts()
{
    if(mMode == MODE_BUILD)
    {
        for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
        {
            if((*i)->getPartType() == mSelectedPartType && (!mSelectedFloorFromEnabled || (*i)->getParentSceneNode()->getPosition().y >= mSelectedFloorFrom*2) && (!mSelectedFloorToEnabled || (*i)->getParentSceneNode()->getPosition().y <= mSelectedFloorTo*2))
            {
                (*i)->getParentSceneNode()->setVisible(true);
            }else
            {
                (*i)->getParentSceneNode()->setVisible(false);
            }

        }
        for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
        {
            SpaceShipPart *part = mSpaceShip->getPart(i);
            if((!mSelectedFloorFromEnabled || part->getParentSceneNode()->getPosition().y >= mSelectedFloorFrom*2) && (!mSelectedFloorToEnabled || part->getParentSceneNode()->getPosition().y <= mSelectedFloorTo*2))
            {
                part->getParentSceneNode()->setVisible(true);
            }else
            {
                part->getParentSceneNode()->setVisible(false);
            }
        }
    }else if(mMode == MODE_WIRE)
    {
        printf("Wiremode\n");
        for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
        {
            (*i)->getParentSceneNode()->setVisible(false);
        }
        for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
        {
            SpaceShipPart *part = mSpaceShip->getPart(i);
            if(part->getType() == "CPU" && (!mLinkFirst || part == mLinkFirst))
            {
                part->getParentSceneNode()->setVisible(true);
            }else if(part->getType().find("CPU_") == 0 && mLinkFirst && !((Hardware *)part)->isConnected())
            {
                part->getParentSceneNode()->setVisible(true);
            }else
            {
                part->getParentSceneNode()->setVisible(false);
            }
        }
    }else if(mMode == MODE_WIRELIGHT)
    {
        for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
        {
            (*i)->getParentSceneNode()->setVisible(false);
        }
        for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
        {
            SpaceShipPart *part = mSpaceShip->getPart(i);
            if(part->getType() == "CPU_LightControl" && (!mLinkFirst || part == mLinkFirst))
            {
                part->getParentSceneNode()->setVisible(true);
            }else if(part->getType() == "SC_SpaceShipPartLight" && mLinkFirst && !((SpaceShipPartLight *)part)->isConnected())
            {
                part->getParentSceneNode()->setVisible(true);
            }else
            {
                part->getParentSceneNode()->setVisible(false);
            }
        }
    }else if(mMode == MODE_WIREDOOR)
    {
        for(std::vector<SpaceShipPart *>::iterator i = mPossibleParts.begin(); i != mPossibleParts.end(); ++i)
        {
            (*i)->getParentSceneNode()->setVisible(false);
        }
        for(size_t i=0; i<mSpaceShip->getNumberOfParts(); i++)
        {
            SpaceShipPart *part = mSpaceShip->getPart(i);
            if(part->getType() == "CPU_DoorControl" && (!mLinkFirst || part == mLinkFirst))
            {
                part->getParentSceneNode()->setVisible(true);
            }else if(part->getType() == "SC_SpaceShipPartDoor" && mLinkFirst && !((SpaceShipPartDoor *)part)->isConnected())
            {
                part->getParentSceneNode()->setVisible(true);
            }else
            {
                part->getParentSceneNode()->setVisible(false);
            }
        }
    }
}

void SpaceShipDesigner::addPossibleParts(SpaceShipPart *part)
{
    //printf("addPossibleParts: %s\n", part->getName().c_str());
    for(size_t i=0; i<part->getNumberNeighbors(); i++)
    {
        SpaceShipPart *neighbor = part->getNeighbor(i);
        if(!neighbor)
        {
            //printf("\tneighbor %d is null\n", i);
            SpaceShipPart::SpaceShipPartInfo *info = part->getNeighborInfo(i);
            Ogre::Quaternion rot = part->getParentSceneNode()->getOrientation() * info->mRot;
            Ogre::Vector3 pos = part->getParentSceneNode()->getPosition() + rot * info->mPos;

            SpaceShipPart *other = NULL;
            for(std::vector<SpaceShipPart *>::iterator j = mPossibleParts.begin(); j != mPossibleParts.end(); ++j)
            {
                SpaceShipPart *part = *j;
                if(part && part->getPartType() == info->mPartType && part->getParentSceneNode()->getPosition().distance(pos) < LENGTH_THRESHOLD)
                {
                    other = part;
                    //printf("\t\tpart %s is new neighbor\n", other->getName().c_str());
                    break;
                }
            }
            if(!other && info->mPlacable)
            {
                char name[32];
                sprintf(name, "DesignerPart%d", mNextPartID);
                mNextPartID++;

                switch(info->mPartType)
                {
                case SpaceShipPart::PART_FLOOR:
                    other = new SpaceShipPartFloor(pos, rot, mSpaceShip->getParentSceneNode(), name, mEngine);
                    break;
                case SpaceShipPart::PART_WALL:
                    other = new SpaceShipPartWall(pos, rot, mSpaceShip->getParentSceneNode(), name, mEngine);
                    break;
                default:
                    printf("invalid part type\n");
                }
                
                if(info->mPartType != mSelectedPartType || (mSelectedFloorFromEnabled && pos.y < mSelectedFloorFrom*2) || (mSelectedFloorToEnabled && pos.y > mSelectedFloorTo*2))
                    other->getParentSceneNode()->setVisible(false);
                other->setMaterial("DesignerPart");
                //printf("\t\tnew part %s is new neighbor\n", other->getName().c_str());
                mPossibleParts.push_back(other);
            }
            if(other)
            {
                part->setNeighbor(other, i);
                for(size_t k = 0; k<other->getNumberNeighbors(); k++)
                {
                    SpaceShipPart::SpaceShipPartInfo *nInfo = other->getNeighborInfo(k);
                    if(nInfo->mPartType == part->getPartType())
                    {
                        if((other->getParentSceneNode()->getPosition() + other->getParentSceneNode()->getOrientation() * nInfo->mRot * nInfo->mPos - part->getParentSceneNode()->getPosition()).length() < LENGTH_THRESHOLD)
                        {
                            other->setNeighbor(part, k);
                            //break;
                        }
                    }
                }
            }
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
    printf("\t\tPos:  %s\n", Ogre::StringConverter::toString(part->getParentSceneNode()->getPosition()).c_str());
    for(size_t i=0; i<part->getNumberNeighbors(); i++)
    {
        SpaceShipPart *neighbor = part->getNeighbor(i);
        if(neighbor)
        {
            if(partType == -1 || partType == neighbor->getPartType())
            {
                printf("\t\t\tNeighbor %d:\n", i);
                printf("\t\t\t\tName: %s\n", neighbor->getName().c_str());
                printf("\t\t\t\tPos:  %s\n", Ogre::StringConverter::toString(neighbor->getParentSceneNode()->getPosition()).c_str());
            }
        }
    }
}
