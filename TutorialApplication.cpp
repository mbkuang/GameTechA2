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
#include <cmath>

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
    simulator = new Simulator();
    aimanager = new AIManager(simulator);
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // Create your scene here :)

    simulator->overlay->initCEGUI();
    mCamera->setPosition(Ogre::Vector3(0,0,100));

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 25, 50);
    light->setDiffuseColour(1.0,1.0,1.0);
    lightNode->attachObject(light);


    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    Wall* flooring = new Wall("Flooring", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, yFWall, -200.0f), Ogre::Vector3(100.0f, 20.0f, 400.0f),
        "WallTexture2Inverse", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* ceiling = new Wall("Ceiling", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, yCWall, -200.0f), Ogre::Vector3(100.0f, 20.0f, 400.0f),
        "WallTexture2Inverse", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* westWall = new Wall("WestWall", mSceneMgr, simulator,
        Ogre::Vector3(xWWall, 0.0f, -200.0f), Ogre::Vector3(20.0f, 80.0f, 400.0f),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* eastWall = new Wall("EastWall", mSceneMgr, simulator,
        Ogre::Vector3(xEWall, 0.0f, -200.0f), Ogre::Vector3(20.0f, 80.0f, 400.0f),
        "WallTextureInverse", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* northWall = new Wall("NorthWall", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, zNWall), Ogre::Vector3(100.0f, 100.0f, 20.0f),
        "WallTextureInvisible", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* southWall = new Wall("SouthWall", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, zSWall), Ogre::Vector3(100.0f, 100.0f, 20.0f),
        "WallTextureInvisible", wallMass, wallRestitution, wallFriction, wallKinematic);

    Player* player1 = new Player("Player1", simulator);
    Player* cpuPlayer = new Player("CPU", simulator);

    Ball* ball = new Ball("Ball", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, -200.0f), 2.0f,
        "BallTexture", ballMass, ballRestitution, ballFriction, ballKinematic);

    Paddle* playerPaddle = new Paddle("PlayerPaddle", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, -1.0f), Ogre::Vector3(16.0f, 15.0f, 1.0f),
        "PaddleTexture", paddleMass, paddleRestitution, paddleFriction, paddleKinematic);

    Paddle* cpuPaddle = new Paddle("CPUPaddle", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, -399.0f), Ogre::Vector3(16.0f, 15.0f, 1.0f),//12.0f, 100.0f, 1.0f),
        "PaddleTexture", paddleMass, paddleRestitution, paddleFriction, paddleKinematic);

    aimanager->update(mSceneMgr, simulator, cpuPaddle, ball);

    simulator->overlay->createScoreboard();
}
//---------------------------------------------------------------------------
bool TutorialApplication::quit() {
    mShutDown = true;
    return true;
}
//---------------------------------------------------------------------------
bool Overlay::quit() {
    return quit();
}
//---------------------------------------------------------------------------
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) {
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}
//---------------------------------------------------------------------------
void TutorialApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mRoot->addFrameListener(this);
}
//---------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
    bool ret = BaseApplication::frameRenderingQueued(fe);

    if(mWindow->isClosed())
        return false;
    if(mShutDown)
        return false;

    //Capture/update devices
    mKeyboard->capture();
    mMouse->capture();

    //Inject timestamps to CEGUI system
    CEGUI::System::getSingleton().injectTimePulse(fe.timeSinceLastFrame);

    if (!processUnbufferedInput(fe))
        return false;

    aimanager->move(fe);
    aimanager->shoot();

    // Update Ogre with Bullet's State
	if (this->simulator != NULL){
		//suppose you have 60 frames per second
        simulator->stepSimulation(fe.timeSinceLastFrame, 60.0f, 1.0f/60.0f);
	}

    Ball* ball = (Ball*) simulator->getObject("Ball");
    ball->getMarkerNode()->setPosition(0, 0, ball->getPosition().getZ());

    return ret;
}
//---------------------------------------------------------------------------
bool TutorialApplication::keyPressed(const OIS::KeyEvent& arg) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    if (arg.key == OIS::KC_ESCAPE) {
        mShutDown = true;
    } else if (arg.key == OIS::KC_R) {
        //Reset the ball's position
        Ball* ball = (Ball*) simulator->getObject("Ball");
        ball->init();
    }
    else if(arg.key == OIS::KC_SPACE) {
        Player* p = simulator->getPlayer("Player1");
        if(!p->hasFired()) {
            GameObject* paddle = simulator->getObject("PlayerPaddle");
            Ogre::Vector3 location = (Ogre::Vector3) paddle->getPosition();

            if(p->getNumShots() == 0) {
                Ball* laser = new Ball("plaser", mSceneMgr, simulator,
                Ogre::Vector3(location.x, location.y, location.z-20), 0.5f,
                "greenball", ballMass, ballRestitution, ballFriction, ballKinematic);
                laser->setVelocity(btVector3(0, 0, -200));
            }
            else {
                Ball* laser = (Ball*) simulator->getObject("plaser");
                laser->setPosition(btVector3(location.x, location.y, location.z-20));
                laser->setVelocity(btVector3(0, 0, -200));
            }
            p->shot();
            simulator->soundSystem->playSound("laserSound");
        }

    }
    else if(arg.key == OIS::KC_UP) {
        simulator->soundSystem->volumeUp();
    }
    else if(arg.key == OIS::KC_DOWN) {
        simulator->soundSystem->volumeDown();
    }
    else if(arg.key == OIS::KC_P) {
        simulator->soundSystem->playSound("bg_music");
    }
    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::keyReleased(const OIS::KeyEvent& arg) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mouseMoved(const OIS::MouseEvent& arg) {
    CEGUI::GUIContext& sys = CEGUI::System::getSingleton().getDefaultGUIContext();
    sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    // Scroll wheel.
    if (arg.state.Z.rel)
        sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);

    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::processUnbufferedInput(const Ogre::FrameEvent& fe)
{
    float movementSpeed = 120.0f;
    Paddle* playerPaddle = (Paddle*) simulator->getObject("PlayerPaddle");

    if (mKeyboard->isKeyDown(OIS::KC_W)) {
        playerPaddle->move(0.0f, movementSpeed * fe.timeSinceLastFrame, 0.0f);
    }

    if (mKeyboard->isKeyDown(OIS::KC_S)) {
        playerPaddle->move(0.0f, -movementSpeed * fe.timeSinceLastFrame, 0.0f);
    }


    if (mKeyboard->isKeyDown(OIS::KC_A)) {
        playerPaddle->move(-movementSpeed * fe.timeSinceLastFrame, 0.0f, 0.0f);
    }

    if (mKeyboard->isKeyDown(OIS::KC_D)) {
        playerPaddle->move(movementSpeed * fe.timeSinceLastFrame, 0.0f, 0.0f);
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

    // playerPaddle->updateTransform();

    btVector3 pPosition = playerPaddle->getPosition();
    Ogre::Vector3 cPosition = mCamera->getPosition();
    cPosition.x = (0 + pPosition.getX())/2;
    cPosition.y = (0 + pPosition.getY())/2;
    mCamera->setPosition(cPosition);

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
