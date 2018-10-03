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
    gameSimulator = new Simulator();
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
    light->setType(Ogre::Light::LT_POINT);
    lightNode->attachObject(light);

    Ball* ball = new Ball(mSceneMgr);
    gameSimulator->addObject(ball);
}
//---------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    bool ret = BaseApplication::frameRenderingQueued(fe);

    if (!processUnbufferedInput(fe))
        return false;

    return ret;
}
//---------------------------------------------------------------------------
bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
    if (mKeyboard->isKeyDown(OIS::KC_R)) {
        //ball->reset();
    }

    if (mKeyboard->isKeyDown(OIS::KC_W)) {
        Ogre::SceneNode* camNode = mSceneMgr->getSceneNode("CameraNode");
        Ogre::Vector3 cPosition = camNode->getPosition();
        Ogre::Vector3 cDirection = mCamera->getDirection();
        cPosition.x += cDirection.x*.05;
        cPosition.y += cDirection.y*.05;
        cPosition.z += cDirection.z*.05;
        mCamera->move(cPosition);
    }

    if (mKeyboard->isKeyDown(OIS::KC_S)) {
        Ogre::SceneNode* camNode = mSceneMgr->getSceneNode("CameraNode");
        Ogre::Vector3 cPosition = camNode->getPosition();
        Ogre::Vector3 cDirection = mCamera->getDirection();
        cPosition.x -= cDirection.x*.05;
        cPosition.y -= cDirection.y*.05;
        cPosition.z -= cDirection.z*.05;
        mCamera->move(cPosition);
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
        mCamera->roll(Ogre::Degree(.05));
    }

    if (mKeyboard->isKeyDown(OIS::KC_D)) {
        mCamera->roll(Ogre::Degree(-.05));
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
