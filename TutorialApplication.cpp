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
    time_passed = 0;
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

    simulator->overlay->createMainMenu();

    Shooter* playerShooter = new Shooter("PlayerShooter", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, -50.0f, -50.0f), Ogre::Vector3(1.5f, 5.0f, 1.5f),
        "ShooterTexture", shooterMass, shooterRestitution, shooterFriction, shooterKinematic);

    createLevel1();
}
//---------------------------------------------------------------------------
void TutorialApplication::createLevel1() {
    /* AI Stuff */
    aiMgr = new AIManager(mSceneMgr, simulator, "greenball");

    /* Adding nodes to map */
    aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -100.0f));
    aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -100.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -100.0f));
    aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -100.0f));
    aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -100.0f));
    aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -80.0f));
    aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -80.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -80.0f));
    aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -80.0f));
    aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -80.0f));
    aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -60.0f));
    aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -60.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -60.0f));
    aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -60.0f));
    aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -60.0f));
    aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -40.0f));
    aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -40.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -40.0f));
    aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -40.0f));
    aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -40.0f));
    aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -20.0f));
    aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -20.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -20.0f));
    aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -20.0f));
    aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -20.0f));
    aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, 0.0f));
    aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, 0.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, 0.0f));
    aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, 0.0f));
    aiMgr->addNode(Ogre::Vector3(-40.0f, -115.0f, 0.0f));
    aiMgr->connectAllNodes();
    // aiMgr->printAllNodeConnections();

    Wall* flooring1 = new Wall("Flooring1", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, yFWall, -50.0f), Ogre::Vector3(100, wallThickness, 100),
        "WallTexture2Inverse", wallMass, wallRestitution, wallFriction, wallKinematic);

    Player* player1 = new Player("Player1", simulator);
    Player* cpuPlayer = new Player("CPU", simulator);

    frog1 = new Frog("Frog1", mSceneMgr, simulator,
        Ogre::Vector3(-40.0f, -135.0f, -100.0f), 2.0f,
        "BallTexture", aiMgr);

    frog2 = new Frog("Frog2", mSceneMgr, simulator,
        Ogre::Vector3(40.0f, -135.0f, -100.0f), 2.0f,
        "BallTexture", aiMgr);

    simulator->overlay->createScoreboard();
}
//---------------------------------------------------------------------------
bool TutorialApplication::quit() {
    mShutDown = true;
    return true;
}
//---------------------------------------------------------------------------
void TutorialApplication::newGame() {
    positions.pHealth = 5;
    positions.eHealth = 5;
    simulator->getPlayer("Player1")->setHP(5);
    simulator->getPlayer("CPU")->setHP(5);
    simulator->pause();
    simulator->overlay->gameOverMenu->hide();
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

    if(simulator->overlay->done)
        mShutDown = true;

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

    // Update Ogre with Bullet's State
	if (this->simulator != NULL && !simulator->paused()) {
		//suppose you have 60 frames per second
        simulator->stepSimulation(fe.timeSinceLastFrame, 1, 1.0f/60.0f);
    }

    // Update the mCamera and player's orientation.
    Shooter* pShooter = (Shooter*) simulator->getObject("PlayerShooter");
    Ogre::Vector3 position = pShooter->getOgrePosition();
    position.y += 2.5f;
    mCamera->setPosition(position - 50 * mCamera->getDirection());

    btQuaternion q = pShooter->getBody()->getOrientation();
    Ogre::Quaternion cQ = mCamera->getOrientation();

    pShooter->getBody()->activate(true);

    float ctheta = std::asin(-2.0f * (cQ.y * cQ.w - cQ.x * cQ.z));
    float theta = std::asin(-2.0f * (q.getY() * q.getW() - q.getX() * q.getZ()));

    float sign = 1.0f;
    if (old_x < 0.0f) {
        sign = -1.0f;
    }
    float spin = 5.0f * sign;

    if (theta >= ctheta - 0.1f && theta <= ctheta + 0.1f) {
        pShooter->getBody()->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
    } else {
        pShooter->getBody()->setAngularVelocity(btVector3(0.0f, spin, 0.0f));
    }

    /* Find player position */
    aiMgr->setPlayerPosition(position);

    return ret;
}
//---------------------------------------------------------------------------
bool TutorialApplication::keyPressed(const OIS::KeyEvent& arg) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    if (arg.key == OIS::KC_ESCAPE) {
        quit();
    } else if (arg.key == OIS::KC_SPACE) {
        Shooter* player = (Shooter*) simulator->getObject("PlayerShooter");
        player->setVelocity(0,50,0);

    } else if (arg.key == OIS::KC_LSHIFT) {
        simulator->destroyWorld();
    } else if(arg.key == OIS::KC_UP) {
        simulator->soundSystem->volumeUp();
    } else if(arg.key == OIS::KC_DOWN) {
        simulator->soundSystem->volumeDown();
    } else if(arg.key == OIS::KC_P) {
        simulator->soundSystem->playSound("bg_music");
    } else if(arg.key == OIS::KC_M) {
        simulator->overlay->pauseGame();
    } else if(arg.key == OIS::KC_L) {
        simulator->printMap();
        simulator->printList();
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

    if (simulator->paused()) {
        return true;
    }

    // Use mouse to control camera
    float cRotSpd = 0.5f;
    old_x = -arg.state.X.rel;
    mCamera->yaw(Ogre::Degree(-arg.state.X.rel * cRotSpd));
    mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * cRotSpd));

    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    if (!simulator->paused()) {
        if (id == OIS::MB_Left) {
            Shooter* player = (Shooter*) simulator->getObject("PlayerShooter");
            Ogre::Vector3 location = player->getGunPosition();
            float avgVel = sqrt(640000/3);
            Ogre::Vector3 cDir = mCamera->getDirection();

            Ogre::stringstream ss;
            ss << "PlayerLaser" << player->getNumShots();

            Laser* laser = new Laser(ss.str(), mSceneMgr, simulator,
            Ogre::Vector3(location.x+cDir.x*1.5, location.y+cDir.y, location.z+cDir.z*1.5), 2.0f,
            "BallTexture", ballMass, ballRestitution, ballFriction, ballKinematic);
            laser->setVelocity(btVector3(avgVel*cDir.x, avgVel*cDir.y, avgVel*cDir.z));

            player->shot();

            positions.xPBPos = location.x+cDir.x;
            positions.yPBPos = location.y+cDir.y;
            positions.zPBPos = location.z+cDir.z;
        }
    }
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
    float movementSpeed = 60.0f;
    Ogre::Degree rotationSpeed = Ogre::Degree(.05);
    Shooter* player = (Shooter*) simulator->getObject("PlayerShooter");

    if (simulator->paused()) {
        return true;
    }

    Ogre::Vector3 cDir = mCamera->getDirection();
    if (cDir.y >= .9) {
        cDir.y = .9;
    }
    if (cDir.y <= -.9) {
        cDir.y = -.9;
    }
    mCamera->setDirection(cDir);

    Ogre::Vector3 moveDir = Ogre::Vector3(0.0f, 0.0f, 0.0f);
    Ogre::Vector3 cDirPerp = Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y) * cDir;

    float walk = (mKeyboard->isKeyDown(OIS::KC_W) - mKeyboard->isKeyDown(OIS::KC_S));
    float strafe = (mKeyboard->isKeyDown(OIS::KC_D) - mKeyboard->isKeyDown(OIS::KC_A));

    if (walk != 0.0f || strafe != 0.0f) {
       player->getBody()->activate(true);
    }

    moveDir.x = ((cDir.x * walk) + (cDirPerp.x * strafe)) * movementSpeed;
    moveDir.z = ((cDir.z * walk) + (cDirPerp.z * strafe)) * movementSpeed;

    float yVelocity = player->getVelocity().getY();

    player->setVelocity(moveDir.x, yVelocity, moveDir.z);

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
