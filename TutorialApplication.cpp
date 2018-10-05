/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
    simulator = new Simulator();
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // Create your scene here :)

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 25, 50);
    light->setDiffuseColour(1.0,1.0,1.0);
    lightNode->attachObject(light);

    //Wall* wall = new Wall("Wall", mSceneMgr, simulator, 0, -50, 0, 1, 1, .5);

    Ball* ball = new Ball("Ball", mSceneMgr, simulator);
    Paddle* playerPaddle = new Paddle("PlayerPaddle", mSceneMgr, simulator);
    playerPaddle->setPosition(0.0,0.0,50.0);
}
//---------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    bool ret = BaseApplication::frameRenderingQueued(fe);

    if (!processUnbufferedInput(fe))
        return false;

    // Update Ogre with Bullet's State
	if (this->simulator != NULL){
		simulator->getDynamicsWorld()->stepSimulation(1.0f/60.0f); //suppose you have 60 frames per second

		for (int i = 0; i < this->simulator->getCollisionObjectCount(); i++) {
			btCollisionObject* obj = this->simulator->getDynamicsWorld()->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);

			if (body && body->getMotionState()){
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);

				void *userPointer = body->getUserPointer();
				if (userPointer) {
					btQuaternion orientation = trans.getRotation();
					Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
					sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
					sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
				}
			}
		}
	}

    return ret;
}
//---------------------------------------------------------------------------
bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
    if (mKeyboard->isKeyDown(OIS::KC_R)) {
        //TODO ball->reset();
    }

    if (mKeyboard->isKeyDown(OIS::KC_W)) {
        GameObject* playerPaddle = simulator->getObject("PlayerPaddle");
        btVector3 pPosition = playerPaddle->getPosition();
        pPosition.setY(pPosition.getY()+.1);
        playerPaddle->setPosition(pPosition);
    }

    if (mKeyboard->isKeyDown(OIS::KC_S)) {
        GameObject* playerPaddle = simulator->getObject("PlayerPaddle");
        btVector3 pPosition = playerPaddle->getPosition();
        pPosition.setY(pPosition.getY()-.1);
        playerPaddle->setPosition(pPosition);
    }

    if (mKeyboard->isKeyDown(OIS::KC_Q)) {
        mCamera->yaw(Ogre::Degree(.05));
    }

    if (mKeyboard->isKeyDown(OIS::KC_E)) {
        mCamera->yaw(Ogre::Degree(-.05));
    }

    if (mKeyboard->isKeyDown(OIS::KC_Z)) {
        mCamera->pitch(Ogre::Degree(.05));
    }

    if (mKeyboard->isKeyDown(OIS::KC_C)) {
        mCamera->pitch(Ogre::Degree(-.05));
    }

    if (mKeyboard->isKeyDown(OIS::KC_A)) {
        GameObject* playerPaddle = simulator->getObject("PlayerPaddle");
        btVector3 pPosition = playerPaddle->getPosition();
        pPosition.setX(pPosition.getX()-.1);
        playerPaddle->setPosition(pPosition);
    }

    if (mKeyboard->isKeyDown(OIS::KC_D)) {
        GameObject* playerPaddle = simulator->getObject("PlayerPaddle");
        btVector3 pPosition = playerPaddle->getPosition();
        pPosition.setX(pPosition.getX()+.1);
        playerPaddle->setPosition(pPosition);
    }

    return true;
}
//---------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
