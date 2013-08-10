#include "Player.hpp"

#include "Input.hpp"

#include "OGRE/OgreSceneManager.h"
#include "OGRE/OgreSceneNode.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRay.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreStringConverter.h"

#include "OIS/OIS.h"

#include "ENGINE.hpp"
#include "KinoControl.hpp"
#include "Kino.hpp"
#include "SpaceShip.hpp"
#include "SpaceShipDesigner.hpp"
#include "SpaceShipPart.hpp"
#include "SpaceShipPartDoor.hpp"
#include "CPUKeyboard.hpp"
#include "GUIManager.hpp"

#include <stdio.h>

#define PLAYER_SIZE (0.5)

#if _MSC_VER
#define snprintf _snprintf
#endif

Player::Player(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::RenderWindow *window, Ogre::String name, ENGINE *engine)
    : Human(pos, ori, parent, name, engine)
{
    mCamera = engine->getSceneMgr()->createCamera(name);
    mCamera->lookAt(0,0,-1);
    mCamera->setPosition(0, 0.3, 0);
    mCameraYawNode   = mNode->createChildSceneNode(name + "YawNode");
    mCameraPitchNode = mCameraYawNode->createChildSceneNode(name + "PitchNode");
    mCameraRollNode  = mCameraPitchNode->createChildSceneNode(name + "RollNode");
    mCameraRollNode->attachObject(mCamera);

    mEntity->getParentSceneNode()->getParentSceneNode()->removeChild(mEntity->getParentSceneNode());
    mCameraYawNode->addChild(mEntity->getParentSceneNode());

    mViewport = window->addViewport(mCamera, 100, 0, 0, 1, 1);

    mViewport->setAutoUpdated(true);
    mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
    engine->getSceneMgr()->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));

    mCamera->setAspectRatio(1.0f * mViewport->getActualWidth() / mViewport->getActualHeight());
    mCamera->setNearClipDistance(0.3);
    mCamera->setFarClipDistance(300.0);
    
    mInput = Input::getSingletonPtr();
    mInput->initialise(window);
    mInput->addKeyListener(this, name);
    mInput->addMouseListener(this, name);

    engine->initGUIManager();
    mInput->addKeyListener(engine->getGUIManager(), "GUIManager");
    mInput->addMouseListener(engine->getGUIManager(), "GUIManager");


    mRaySceneQuery = engine->getSceneMgr()->createRayQuery(Ogre::Ray());

    mMode = MODE_DEFAULT;

    mTranslation = Ogre::Vector3::ZERO;

    mSpaceShipDesigner = new SpaceShipDesigner(engine);
}

bool Player::update(float elapsedTime)
{
    mInput->capture();
    
    if(mInput->getKeyboard()->isKeyDown(OIS::KC_ESCAPE))
	{
        return false;
	}

    //movement
    if(mTranslation != Ogre::Vector3::ZERO && mMode == MODE_DEFAULT)
	{
        float speed = 5.0f * elapsedTime;
        Ogre::Vector3 translation = mTranslation;
        translation = mCameraYawNode->getOrientation() * mCameraPitchNode->getOrientation() * translation;
        translation.y = 0;
        translation.normalise();

        // collision detection
        Ogre::Ray ray(mNode->getParentSceneNode()->getPosition() + mNode->getPosition(), translation);
        mRaySceneQuery->setRay(ray);
        mRaySceneQuery->setSortByDistance(true);

        Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = result.begin();

        while(i!=result.end())
        {
            if(i->distance-PLAYER_SIZE < speed)
            {
                if(i->movable)
                {
                    if(i->movable->getMovableType() == "Entity" && i->movable->getName() != mNode->getName() + "Mesh")
                    {
                        Entity *ent = Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
                        if(ent && ent->getType() == "SC_SpaceShipPartDoor" && ((SpaceShipPartDoor *)ent)->isOpen())
                            continue;
                        printf("colliding with %s", i->movable->getName().c_str());
                        speed = i->distance - PLAYER_SIZE;
                        break;
                    }
                }
            }else
                break;
            ++i;
        }
        
        translation = translation * speed;
        mNode->translate(translation, Ogre::Node::TS_LOCAL);
    }
        
    // test: ship movement
    {
        float speed = 200.0f * elapsedTime;
	    Ogre::Vector3 translation(Ogre::Vector3::ZERO);
        if(mInput->getKeyboard()->isKeyDown(OIS::KC_UP))
	    {
		    translation.z -= 1;
	    }
        if(mInput->getKeyboard()->isKeyDown(OIS::KC_DOWN))
	    {
		    translation.z += 1;
	    }
        if(mInput->getKeyboard()->isKeyDown(OIS::KC_LEFT))
	    {
		    translation.x -= 1;
	    }
	    if(mInput->getKeyboard()->isKeyDown(OIS::KC_RIGHT))
	    {
		    translation.x += 1;
	    }
	    if(translation != Ogre::Vector3::ZERO)
	    {
            translation.normalise();
            mNode->getParentSceneNode()->translate(translation * speed, Ogre::Node::TS_LOCAL);
	    }
    }
    return true;
}

bool Player::keyPressed(const OIS::KeyEvent &e)
{
    switch(e.key)
    {
    case OIS::KC_W:
        mTranslation.z = -1;
        break;
    case OIS::KC_S:
        mTranslation.z = 1;
        break;
    case OIS::KC_A:
		mTranslation.x = -1;
        break;
    case OIS::KC_D:
		mTranslation.x = 1;
        break;
    case OIS::KC_F1:
        if(mMode == MODE_DEFAULT)
        {
            mMode = MODE_DESIGN;
            mEngine->getWindow()->removeViewport(100);
            mSpaceShipDesigner->enterEditMode(mShip);

            mInput->addKeyListener(mSpaceShipDesigner, "SpaceShipDesigner");
            mInput->addMouseListener(mSpaceShipDesigner, "SpaceShipDesigner");
        }else if(mMode == MODE_DESIGN)
        {
            mMode = MODE_DEFAULT;
            mSpaceShipDesigner->exitEditMode();
            mViewport = mEngine->getWindow()->addViewport(mCamera, 100, 0, 0, 1, 1);
            mViewport->setAutoUpdated(true);
            mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
            mEngine->getSceneMgr()->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));
            mInput->removeKeyListener("SpaceShipDesigner");
            mInput->removeMouseListener("SpaceShipDesigner");
        }
        return false;
    case OIS::KC_E:
        if(mMode == MODE_DEFAULT || mMode == MODE_KINOCONTROL || mMode == MODE_KEYBOARD)
        {
            Ogre::Ray ray(mNode->getParentSceneNode()->getPosition() + mNode->getPosition(), mCameraYawNode->getOrientation() * mCameraPitchNode->getOrientation() * Ogre::Vector3(0,0,-1));
            mRaySceneQuery->setRay(ray);
            mRaySceneQuery->setSortByDistance(true);

            Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
            Ogre::RaySceneQueryResult::iterator i = result.begin();

            while(i!=result.end())
            {
                if(i->distance > 10)
                    break;
            
                if(i->movable && i->movable->getMovableType() == "Entity" && i->movable->getName() != mNode->getName() + "Mesh")
                {
                    Entity *obj = Ogre::any_cast<Entity *>(i->movable->getUserObjectBindings().getUserAny("Entity"));
                    if(obj)
                    {           
                        printf("hit %s", obj->getType().c_str());
                        if(obj->getType() == "SC_KinoControl")
                        {
                            Kino *kino = ((KinoControl *)obj)->getKino();
                            if(mMode == MODE_DEFAULT)
                            {
                                mMode = MODE_KINOCONTROL;
                                mInput->addKeyListener(kino, kino->getName());
                                mInput->addMouseListener(kino, kino->getName());
                            }else if(mMode == MODE_KINOCONTROL)
                            {
                                mMode = MODE_DEFAULT;
                                kino->stop();
                                mInput->removeKeyListener(kino->getName());
                                mInput->removeMouseListener(kino->getName());
                            }
                            return false; // stop iterating the Input->mKeyListeners map since we changed it
                        }else if(obj->getType() == "CPU_Keyboard")
                        {
                            CPUKeyboard *keyboard = ((CPUKeyboard *)obj);
                            if(mMode == MODE_DEFAULT)
                            {
                                mMode = MODE_KEYBOARD;
                                mInput->addKeyListener(keyboard, keyboard->getName());
                            }else if(mMode == MODE_KEYBOARD)
                            {
                                mMode = MODE_DEFAULT;
                                mInput->removeKeyListener(keyboard->getName());
                            }
                            return false; // stop iterating the Input->mKeyListeners map since we changed it
                        }else if(obj->getType() == "SC_SpaceShipPartDoor")
                        {
                            if(mMode == MODE_DEFAULT)
                            {
                                printf("using door\n");
                                SpaceShipPartDoor *door = ((SpaceShipPartDoor *)obj);
                                door->open(!door->isOpen());
                            }
                            return true;
                        }
                    }
                }
                ++i;
            }
        }
        break;
    }
    return true;
}

bool Player::keyReleased(const OIS::KeyEvent &e)
{
    switch(e.key)
    {
    case OIS::KC_W:
    case OIS::KC_S:
        mTranslation.z = 0;
        break;
    case OIS::KC_A:
    case OIS::KC_D:
		mTranslation.x = 0;
        break;
    }
    return true;
}
 
bool Player::mouseMoved(const OIS::MouseEvent &e)
{
    if(mMode != MODE_DEFAULT)
        return true;
    //orientation
    float rotCoeff = -1.0f * 0.005;// elapsedTime;
    Ogre::Radian lAngleX(e.state.X.rel * rotCoeff);
    Ogre::Radian lAngleY(e.state.Y.rel * rotCoeff);
	// If the 'player' don't make loopings, 'yaw in world' + 'pitch in local' is often enough for a camera controler.
    mCameraYawNode->yaw(lAngleX);
    mCameraPitchNode->pitch(lAngleY);

    Ogre::Real pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(mCameraPitchNode->getOrientation().w)).valueDegrees());
 
    // Just to determine the sign of the angle we pick up above, the
    // value itself does not interest us.
    Ogre::Real pitchAngleSign = mCameraPitchNode->getOrientation().x;
 
    // Limit the pitch between -90 degress and +90 degrees, Quake3-style.
    if (pitchAngle > 90.0f)
    {
        if (pitchAngleSign > 0)
            // Set orientation to 90 degrees on X-axis.
             mCameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
        else if (pitchAngleSign < 0)
            // Sets orientation to -90 degrees on X-axis.
            mCameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
    }

    
    switch(mMode)
    {
    case MODE_DEFAULT:
        break;
    case MODE_DESIGN:
        /*
        mNextPartPos += e.state.Z.rel/120;
        mNextPartPos %= mNextPart->getNumberNeighbors();
        */
        break;
    }

    return true;
}

bool Player::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    switch(mMode)
    {
    case MODE_DEFAULT:
        break;
    case MODE_DESIGN:
        /*
        if(id == OIS::MB_Left)
        {
            if(mNextPart->isVisible())
            {
                mShip->addPart(mNextPart);
                char nextName[128];
                snprintf(nextName, 128, "%sPart%d",  mShip->getName().c_str(), mShip->getNumberOfParts());
                printf("createing part %s\n", nextName);
                mNextPart = new SpaceShipPart(SpaceShipPart::PART_FLOOR, Ogre::Vector3(0, -2, 0), Ogre::Quaternion(), mShip->getSceneNode(), std::string(nextName), mEngine);
                mNextPart->getSceneNode()->setVisible(false);
            }
        }
        */
        break;
    }
    return true;
}

bool Player::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    return true;
}
