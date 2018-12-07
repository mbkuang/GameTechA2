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
    aiMgr = new AIManager(mSceneMgr, simulator, "greenball");
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
    mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");

    simulator->overlay->initCEGUI();

    mCamera->setPosition(Ogre::Vector3(0,0,100));

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // create a particle system named Trails using the trail
    Ogre::ParticleSystem* particleSystem = mSceneMgr->createParticleSystem("Trails", "Trail");

    // fast forward 1 second  to the point where the particle has been emitted
    particleSystem->fastForward(1.0);

    // attach the particle system to a scene node
    Ogre::SceneNode* particleNode = mSceneMgr->getRootSceneNode()
        ->createChildSceneNode("ParticleSystem", Ogre::Vector3(0.0f, 0.0f, 0.0f));
    particleNode->attachObject(particleSystem);

    simulator->overlay->createMainMenu();

    Shooter* playerShooter = new Shooter("PlayerShooter", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 40.0f, 0.0f), Ogre::Vector3(1.5f, 5.0f, 1.5f),
        "ShooterTexture", shooterMass, shooterRestitution, shooterFriction, shooterKinematic);

    createLevel1();

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 25, 50);
    light->setDiffuseColour(1.0,1.0,1.0);
    lightNode->attachObject(light);

    Player* player1 = new Player("Player1", simulator);
    Player* cpuPlayer = new Player("CPU", simulator);

    simulator->overlay->createScoreboard();

}
//---------------------------------------------------------------------------
void TutorialApplication::restart() {
    simulator->getPlayer("Player1")->setLevel(0);
    level = 0;
    nextLevel();
}
//---------------------------------------------------------------------------
void TutorialApplication::nextLevel() {
    simulator->pause();
    // printf("Going to next world\n");
    simulator->destroyWorld();
    // printf("Destroyed world\n");
    Ogre::ParticleSystem* particleSystem = mSceneMgr->getParticleSystem("Trails");
    particleSystem->removeAllEmitters();
    // printf("Destroying nodes\n");
    aiMgr->destroyNodes();
    // printf("Got those  nodes\n");
    level ++;
    simulator->getPlayer("Player1")->setLevel(level);
    simulator->pause();
    switch(level) {
        case 1:
            createLevel1();
            break;
        case 2:
            createLevel2();
            break;
        case 3:
            createLevel3();
            break;
        case 4:
            createLevel4();
            break;
        default:
            break;
    }
}
//---------------------------------------------------------------------------
void TutorialApplication::createLevel1() {

    Wall* ground = new Wall("Ground", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, -10.0f, -50.0f), Ogre::Vector3(100, wallThickness, 100),
        "Examples/GrassFloor", wallMass, wallRestitution, wallFriction, wallKinematic);

    Wall* testWall = new Wall("TestWall", mSceneMgr, simulator,
        Ogre::Vector3(-35.0f, 10.0f, -50.0f), Ogre::Vector3(30, wallThickness, 10),
        "Examples/Rockwall", wallMass, wallRestitution, wallFriction, wallKinematic);

    Shooter* player = (Shooter*) simulator->getObject("PlayerShooter");
    player->setPosition(0.0f, 10.0f, -5.0f);
    player->setStartPos(Ogre::Vector3(0.0f, 10.0f, -5.0f));

    Door* door = new Door("Door", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, -95.0f), Ogre::Vector3(10.0f, 10.0f, 10.0f),
        "DoorTexture", 10000, 0.98f, wallFriction, ballKinematic);
}
//---------------------------------------------------------------------------
void TutorialApplication::createLevel2() {

    Wall* flooring1 = new Wall("Flooring1", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, -10.0f, -50.0f), Ogre::Vector3(10, wallThickness, 100),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* flooring2 = new Wall("Flooring2", mSceneMgr, simulator,
        Ogre::Vector3(-45.0f, -10.0f, -105.0f), Ogre::Vector3(100, wallThickness, 10),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);

    Shooter* player = (Shooter*) simulator->getObject("PlayerShooter");
    player->setPosition(0.0f, 10.0f, -50.0f);

    Ogre::ParticleSystem* particleSystem = mSceneMgr->getParticleSystem("Trails");

    Ogre::ParticleEmitter* emitter1 = particleSystem->addEmitter("Point");
    Bird* bird1 = new Bird("Bird1", mSceneMgr, simulator,
        Ogre::Vector3(0, 10.0f, -300.0f), 2.0f,
        "BallTexture", ballMass, ballRestitution, ballFriction, ballKinematic, emitter1);
    bird1->setTarget((Shooter*) simulator->getObject("PlayerShooter"));

    Ogre::ParticleEmitter* emitter2 = particleSystem->addEmitter("Point");
    Bird* bird2 = new Bird("Bird2", mSceneMgr, simulator,
        Ogre::Vector3(0, 10.0f, 300.0f), 2.0f,
        "greenball", ballMass, ballRestitution, ballFriction, ballKinematic, emitter2);
    bird2->setTarget((Shooter*) simulator->getObject("PlayerShooter"));
    bird2->setLeader(bird1->getName());
    bird2->setFormation(btVector3(-75.0f,0.0f,0.0f));

    Ogre::ParticleEmitter* emitter3 = particleSystem->addEmitter("Point");
    Bird* bird3 = new Bird("Bird3", mSceneMgr, simulator,
        Ogre::Vector3(20.0f, 40.0f, -300.0f), 2.0f,
        "greenball", ballMass, ballRestitution, ballFriction, ballKinematic, emitter3);
    bird3->setTarget((Shooter*) simulator->getObject("PlayerShooter"));
    bird3->setLeader(bird1->getName());
    bird3->setFormation(btVector3(0.0f,75.0f,0.0f));

    Door* door = new Door("Door", mSceneMgr, simulator,
        Ogre::Vector3(-45.0f, 0.0f, -105.0f), Ogre::Vector3(10.0f, 10.0f, 10.0f),
        "DoorTexture", 10000, 0.98f, wallFriction, ballKinematic);

}
//---------------------------------------------------------------------------
void TutorialApplication::createLevel3() {
    /* AI Stuff */
    aiMgr = new AIManager(mSceneMgr, simulator, "greenball");

    /* Adding nodes to map */
    // aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -100.0f));
    // aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -100.0f));
    // aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -100.0f));
    // aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -100.0f));
    // aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -100.0f));
    // aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -80.0f));
    // aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -80.0f));
    // aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -80.0f));
    // aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -80.0f));
    // aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -80.0f));
    // aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -60.0f));
    // aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -60.0f));
    // aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -60.0f));
    // aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -60.0f));
    // aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -60.0f));
    // aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -40.0f));
    // aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -40.0f));
    // aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -40.0f));
    // aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -40.0f));
    // aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -40.0f));
    // aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, -20.0f));
    // aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, -20.0f));
    // aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, -20.0f));
    // aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, -20.0f));
    // aiMgr->addNode(Ogre::Vector3(-40.0f, -135.0f, -20.0f));
    // aiMgr->addNode(Ogre::Vector3(40.0f, -135.0f, 0.0f));
    // aiMgr->addNode(Ogre::Vector3(20.0f, -135.0f, 0.0f));
    // aiMgr->addNode(Ogre::Vector3(0.0f, -135.0f, 0.0f));
    // aiMgr->addNode(Ogre::Vector3(-20.0f, -135.0f, 0.0f));
    // aiMgr->addNode(Ogre::Vector3(-40.0f, -115.0f, 0.0f));

    /* If nodes are too far away bad things happen */
    aiMgr->addNode(Ogre::Vector3(50.0f,50.0f,0.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f,50.0f,50.0f));
    aiMgr->addNode(Ogre::Vector3(-50.0f,50.0f,0.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f,50.0f,-50.0f));
    aiMgr->addNode(Ogre::Vector3(50.0f,50.0f,50.0f));
    aiMgr->addNode(Ogre::Vector3(-50.0f,50.0f,50.0f));
    aiMgr->addNode(Ogre::Vector3(-50.0f,50.0f,-50.0f));
    aiMgr->addNode(Ogre::Vector3(50.0f,50.0f,-50.0f));
    aiMgr->addNode(Ogre::Vector3(25.0f,-8.0f,0.0f));
    aiMgr->addNode(Ogre::Vector3(25.0f,-8.0f,25.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f,-8.0f,25.0f));
    aiMgr->addNode(Ogre::Vector3(-25.0f, -8.0f,0.0f));
    aiMgr->addNode(Ogre::Vector3(0.0f,-8.0f,-25.0f));
    aiMgr->addNode(Ogre::Vector3(25.0f, -8.0f,-25.0f));
    aiMgr->addNode(Ogre::Vector3(-25.0f, -8.0f,25.0f));
    aiMgr->addNode(Ogre::Vector3(-25.0f,-8.0f,-25.0f));
    aiMgr->connectAllNodes();

    Wall* flooring1 = new Wall("Wall1", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, -20.0f, 0.0f), Ogre::Vector3(100, wallThickness, 100),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);

    Wall* wall2 = new Wall("Wall2", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 40.0f, -60.0f), Ogre::Vector3(100, 100, wallThickness),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);

    Wall* wall3 = new Wall("Wall3", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 40.0f, 60.0f), Ogre::Vector3(100, 100, wallThickness),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);

    Wall* wall4 = new Wall("Wall4", mSceneMgr, simulator,
        Ogre::Vector3(60.0f, 40.0f, 0.0f), Ogre::Vector3(wallThickness, 100, 100),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);

    Player* player1 = new Player("Player1", simulator);
    Player* cpuPlayer = new Player("CPU", simulator);

    frog1 = new Frog("Frog1", mSceneMgr, simulator,
        Ogre::Vector3(25.0f,-8.0f,0.0f), 4.0f,
        "BallTexture", aiMgr);

    frog2 = new Frog("Frog2", mSceneMgr, simulator,
        Ogre::Vector3(-25.0f,-8.0f,-25.0f), 4.0f,
        "BallTexture", aiMgr);

    Door* door = new Door("Door", mSceneMgr, simulator,
        Ogre::Vector3(50.0f, 150.0f, 50.0f), Ogre::Vector3(10.0f, 10.0f, 10.0f),
        "DoorTexture", 10000, 0.98f, wallFriction, ballKinematic);
}
//---------------------------------------------------------------------------
void TutorialApplication::createLevel4() {

    Wall* flooring1 = new Wall("Flooring1", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, -50.0f), Ogre::Vector3(10.0f, wallThickness, 100.0f),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* flooring2 = new Wall("Flooring2", mSceneMgr, simulator,
        Ogre::Vector3(45.0f, 0.0f, 105.0f), Ogre::Vector3(100.0f, wallThickness, 10.0f),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);

    Shooter* player = (Shooter*) simulator->getObject("PlayerShooter");
    player->setPosition(0.0f, 10.0f, -50.0f);

    Ogre::ParticleSystem* particleSystem = mSceneMgr->getParticleSystem("Trails");

    Ogre::ParticleEmitter* emitter1 = particleSystem->addEmitter("Point");
    Bird* bird1 = new Bird("Bird1", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 10.0f, -300.0f), 2.0f,
        "BallTexture", ballMass, ballRestitution, ballFriction, ballKinematic, emitter1);
    bird1->setTarget((Shooter*) simulator->getObject("PlayerShooter"));

    Ogre::ParticleEmitter* emitter2 = particleSystem->addEmitter("Point");
    Bird* bird2 = new Bird("Bird2", mSceneMgr, simulator,
        Ogre::Vector3(20.0f, 10.0f, -300.0f), 2.0f,
        "WallTexture", ballMass, ballRestitution, ballFriction, ballKinematic, emitter2);
    bird2->setTarget((Shooter*) simulator->getObject("PlayerShooter"));

    Door* door = new Door("Door", mSceneMgr, simulator,
        Ogre::Vector3(45.0f, 10.0f, 105.0f), Ogre::Vector3(10.0f, 10.0f, 10.0f),
        "DoorTexture", 10000, 0.98f, wallFriction, ballKinematic);

    Spawner* spawn = new Spawner("Spawner", mSceneMgr, simulator,
        Ogre::Vector3(30.0f, 10.0f, -300.0f), Ogre::Vector3(10.0f, 10.0f, 10.0f),
        "ShooterTexture", wallMass, 0.98f, wallFriction, ballKinematic, 0, 10.0f, particleSystem);
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

    simulator->bulletTimer();
    Player* player = simulator->getPlayer("Player1");
    Shooter* pShooter = (Shooter*) simulator->getObject("PlayerShooter");
    Ogre::Vector3 startPosition = pShooter->getStartPos();
    Ogre::Vector3 currentPos = (Ogre::Vector3) pShooter->getPosition();
    if(currentPos.y - startPosition.y < -200) {
        if (!pShooter->hasFallenOff()) {
            pShooter->setPosition(startPosition.x, startPosition.y, startPosition.z);
            player->incrementScore();
            player->setHP(5);
            simulator->overlay->updateScore();
            pShooter->setFallenOff(true);
        }
    } else {
        pShooter->setFallenOff(false);
    }

    CEGUI::Window *restartButton = simulator->overlay->mainMenu->getChildRecursive("singlePlayerButton");
    restartButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::restart, this));

    // Update the mCamera and player's orientation.
    Ogre::Vector3 position = pShooter->getOgrePosition();
    position.y += 2.5f;

    if(!firstPerson)
        mCamera->setPosition(position - 20 * mCamera->getDirection());
    else
        mCamera->setPosition(position);

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

    Door* door = (Door*) simulator->getObject("Door");
    if (door != NULL) {
        if (door->tripped) {
            door->tripped = false;
            simulator->overlay->showWinMessage(level);
            nextLevel();
        }
    }

    // if(pShooter->reachedDoor()) {
    //     pShooter->setDoor(false);
    //     simulator->overlay->showWinMessage(level);
    //     nextLevel();
    // }

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
        if(player->canJump()) {
            player->setVelocity(0,50,0);
            player->setJump(false);
        }

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
            float avgVel = laserSpeed;
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
        else if (id == OIS::MB_Right) {
            firstPerson = !firstPerson;
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
    if (cDir.y >= .95) {
        cDir.y = .95;
    }
    if (cDir.y <= -.95) {
        cDir.y = -.95;
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
    // if (moveDir.x == 0) {moveDir.x = 0;}
    moveDir.z = ((cDir.z * walk) + (cDirPerp.z * strafe)) * movementSpeed;

    float yVelocity = player->getVelocity().getY();

    player->setVelocity(moveDir.x, yVelocity/*0.0*/, moveDir.z);

    // btVector3 pVel = player->getVelocity();
    // if (pVel.length() > movementSpeed) {player->setVelocity(pVel.normalized()*movementSpeed);}

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
