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

#include <stdio.h>

#if _MSC_VER
#define snprintf _snprintf
#endif

Player::Player(Ogre::Vector3 pos, Ogre::Quaternion ori, Ogre::SceneNode *parent, Ogre::RenderWindow *window, Ogre::String name, ENGINE *engine)
    : Entity(pos, ori, parent, name, "SC_Player", engine)
{
    mCamera = engine->getSceneMgr()->createCamera(name);
    mCamera->lookAt(0,0,-10);
    mCameraYawNode   = mNode->createChildSceneNode();
    mCameraPitchNode = mCameraYawNode->createChildSceneNode();
    mCameraRollNode  = mCameraPitchNode->createChildSceneNode();
    mCameraRollNode->attachObject(mCamera);

    mViewport = window->addViewport(mCamera, 100, 0, 0, 1, 1);

    mViewport->setAutoUpdated(true);
    mViewport->setBackgroundColour(Ogre::ColourValue(1,.5,.8));

    mCamera->setAspectRatio(1.0f * mViewport->getActualWidth() / mViewport->getActualHeight());
    mCamera->setNearClipDistance(0.5);
    mCamera->setFarClipDistance(5000.0);

    mInput = Input::getSingletonPtr();
    mInput->initialise(window);
    mInput->addKeyListener(this, name);
    mInput->addMouseListener(this, name);

    mRaySceneQuery = engine->getSceneMgr()->createRayQuery(Ogre::Ray());

    mActiveKeyListener = NULL;
    mActiveMouseListener = NULL;

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
    if(mTranslation != Ogre::Vector3::ZERO && !mActiveKeyListener)
	{
        float speed = 5.0f * elapsedTime;
        Ogre::Vector3 translation = mTranslation;
        translation = mCameraYawNode->getOrientation() * mCameraPitchNode->getOrientation() * translation;
        translation.y = 0;
        translation.normalise();
        mNode->translate(translation * speed, Ogre::Node::TS_LOCAL);
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

    if(mMode == MODE_DESIGN)
    {
        /*
        Ogre::Ray ray(mNode->getParentSceneNode()->getPosition() + mNode->getPosition(), mCameraYawNode->getOrientation() * mCameraPitchNode->getOrientation() * Ogre::Vector3(0,0,-1));
        mRaySceneQuery->setRay(ray);
        mRaySceneQuery->setSortByDistance(true);

        Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator i = result.begin();
                
        mNextPart->getSceneNode()->setVisible(false);

        while(i!=result.end())
        {
            if(i->distance > 50)
                break;
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
                            if(mNextPart != obj)
                            {
                                Ogre::Vector3 relPos;
                                switch(mNextPartPos)
                                {
                                case 0:
                                    relPos = Ogre::Vector3(1,0,0);
                                    break;
                                case 1:
                                    relPos = Ogre::Vector3(0,0,1);
                                    break;
                                case 2:
                                    relPos = Ogre::Vector3(-1,0,0);
                                    break;
                                case 3:
                                    relPos = Ogre::Vector3(0,0,-1);
                                    break;
                                }
                                mNextPart->getSceneNode()->setPosition(obj->getSceneNode()->getPosition() + relPos);
                                mNextPart->getSceneNode()->setVisible(true);
                            }
                            break;
                        }
                    }
                }
            }
            ++i;
        }
            */
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
            if(mActiveKeyListener)
            {
                mInput->removeKeyListener(mActiveKeyListener);
            }
            mActiveKeyListener = mSpaceShipDesigner;
            mInput->addKeyListener(mActiveKeyListener, "SpaceShipDesigner");
            if(mActiveMouseListener)
            {
                mInput->removeMouseListener(mActiveMouseListener);
            }
            mActiveMouseListener = mSpaceShipDesigner;
            mInput->addMouseListener(mActiveMouseListener, "SpaceShipDesigner");
        }else if(mMode == MODE_DESIGN)
        {
            mMode = MODE_DEFAULT;
            mSpaceShipDesigner->exitEditMode();
            mViewport = mEngine->getWindow()->addViewport(mCamera, 100, 0, 0, 1, 1);
            mViewport->setAutoUpdated(true);
            mViewport->setBackgroundColour(Ogre::ColourValue(1,.5,.8));
            mInput->removeKeyListener(mActiveKeyListener);
            mActiveKeyListener = NULL;
            mInput->removeMouseListener(mActiveMouseListener);
            mActiveMouseListener = NULL;
        }
        return false;
    case OIS::KC_E:
        if(mMode == MODE_DEFAULT)
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
            
                if(i->movable && i->movable->getMovableType() == "Entity")
                {
                    Ogre::SceneNode *node = i->movable->getParentSceneNode();
                    if(node)
                    {
                        Entity *obj = dynamic_cast<Entity *>(node->getAttachedObject(node->getName() + "Obj"));
                        if(obj)
                        {                
                            if(obj->getType() == "SC_KinoControl")
                            {
                                if(mActiveKeyListener)
                                {
                                    ((Kino *)mActiveKeyListener)->stop();
                                    mInput->removeKeyListener(mActiveKeyListener);
                                    mActiveKeyListener = NULL;
                                }else
                                {
                                    KinoControl *control = (KinoControl *)obj;
                                    mActiveKeyListener = control->getKino();
                                    mInput->addKeyListener((Kino *)mActiveKeyListener, control->getName());
                                }
                                if(mActiveMouseListener)
                                {
                                    ((Kino *)mActiveMouseListener)->stop();
                                    mInput->removeMouseListener(mActiveMouseListener);
                                    mActiveMouseListener = NULL;
                                }else
                                {
                                    KinoControl *control = (KinoControl *)obj;
                                    mActiveMouseListener = control->getKino();
                                    mInput->addMouseListener((Kino *)mActiveMouseListener, control->getName());
                                }
                                return false; // stop iterating the Input->mKeyListeners map since we changed it
                            }
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
    if(mActiveMouseListener)
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
