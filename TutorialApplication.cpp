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

    port_number = 51215;    // Default port
    bool netStarted = false;
    bool connectionMade = false;
    bool multiPlayerStarted = false;
    isMultiplayer = false;
    gameStarted = false;

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

    CEGUI::Window *hostButton = simulator->overlay->multiMenu->getChildRecursive("HostButton");
    hostButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::hostGame, this));

    CEGUI::Window *joinButton = simulator->overlay->multiMenu->getChildRecursive("JoinButton");
    joinButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::joinGame, this));

    createObjects();
}
//---------------------------------------------------------------------------
void TutorialApplication::createObjects() {
    float zMid = -150.0f;

    Wall* flooring = new Wall("Flooring", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, yFWall, 0.0f), Ogre::Vector3(wallWidth, wallThickness, wallLength),
        "WallTexture2Inverse", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* ceiling = new Wall("Ceiling", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, yCWall, zMid), Ogre::Vector3(wallWidth, wallThickness, wallLength),
        "WallTexture2Inverse", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* westWall = new Wall("WestWall", mSceneMgr, simulator,
        Ogre::Vector3(xWWall, 0.0f, zMid), Ogre::Vector3(wallThickness, wallWidth-20.0f, wallLength),
        "WallTexture", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* eastWall = new Wall("EastWall", mSceneMgr, simulator,
        Ogre::Vector3(xEWall, 0.0f, zMid), Ogre::Vector3(wallThickness, wallWidth-20.0f, wallLength),
        "WallTextureInverse", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* northWall = new Wall("NorthWall", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, zNWall), Ogre::Vector3(wallWidth, wallWidth, wallThickness),
        "WallTextureInvisible", wallMass, wallRestitution, wallFriction, wallKinematic);
    Wall* southWall = new Wall("SouthWall", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, zSWall), Ogre::Vector3(wallWidth, wallWidth, wallThickness),
        "WallTextureInvisible", wallMass, wallRestitution, wallFriction, wallKinematic);

    Player* player1 = new Player("Player1", simulator);
    Player* cpuPlayer = new Player("CPU", simulator);

    // Ball* ball = new Ball("Ball", mSceneMgr, simulator,
    //     Ogre::Vector3(0.0f, 0.0f, zMid), 2.0f,
    //     "BallTexture", ballMass, ballRestitution, ballFriction, ballKinematic);

    // Player positional/orientation/bullet pos coords
    positions.xPPos = 0.0f; positions.yPPos = 0.0f; positions.zPPos = -1.0f;
    positions.xPDir = 0.0f; positions.yPDir = 0.0f; positions.zPDir = -1.0f;
    positions.xPBPos = 100.0f; positions.yPBPos = -50.0f; positions.zPBPos = 0.0f;
    // // Enemy positional/orientation/ bullet pos coords;
    positions.xEPos = 0.0f; positions.yEPos = 0.0f; positions.zEPos = -50.0f;
    positions.xEDir = 0.0f; positions.yEDir = 0.0f; positions.zEDir = 1.0f;
    positions.xEBPos = -100.0f; positions.yEBPos = -50.0f; positions.zEBPos = 0.0f;

    std::string s = getPositionString();
    printf("Pos string: %s\n", s.c_str());
    decodePositionString(s);
    printf("Decoded string: %s\n", getPositionString().c_str());

    Ogre::Vector3 shooterPosition;
    Ogre::Vector3 enemyPosition;
    if(isHost || !isMultiplayer) {
        shooterPosition = Ogre::Vector3(positions.xPPos, positions.yPPos, positions.zPPos);
        enemyPosition = Ogre::Vector3(positions.xEPos, positions.yEPos, positions.zEPos);
    }
    else {
        shooterPosition = Ogre::Vector3(positions.xEPos, positions.yEPos, positions.zEPos);
        enemyPosition = Ogre::Vector3(positions.xPPos, positions.yPPos, positions.zPPos);
    }

    Shooter* playerShooter = new Shooter("PlayerShooter", mSceneMgr, simulator,
        shooterPosition, Ogre::Vector3(0.5f, 1.0f, 0.5f),
        "PaddleTexture", shooterMass, shooterRestitution, shooterFriction, shooterKinematic);

    EnemyShooter* cpuShooter = new EnemyShooter("CPUShooter", mSceneMgr, simulator,
        Ogre::Vector3(0.0f, 0.0f, -50.0f), Ogre::Vector3(0.5f, 1.0f, 0.5f),//12.0f, 100.0f, 1.0f),
        "PaddleTexture", enemyShooterMass, shooterRestitution, shooterFriction, enemyShooterKinematic);

    // aimanager->update(mSceneMgr, simulator, cpuPaddle, playerPaddle, ball);

    simulator->overlay->createScoreboard();
}
//---------------------------------------------------------------------------
bool TutorialApplication::quit() {
    closeNetwork();
    mShutDown = true;
    return true;
}
//---------------------------------------------------------------------------
bool TutorialApplication::setupNetwork(bool isHost) {
    this->isHost = isHost;
    bool success;
    success = network.initNetManager();
    netStarted = true;
    if(isHost) {
        network.addNetworkInfo(PROTOCOL_TCP, NULL, port_number);
        //TODO
        //network.addNetworkInfo(PROTOCOL_UDP, NULL, port_number);
        network.acceptConnections();
        success = network.startServer();
    }
    else {
        network.addNetworkInfo(PROTOCOL_TCP, hostName, port_number);
        //TODO
        //network.addNetworkInfo(PROTOCOL_UDP, hostName, port_number);
        success = network.startClient();
    }
    return success;
}
//---------------------------------------------------------------------------
void TutorialApplication::closeNetwork() {
    network.close();
}
//---------------------------------------------------------------------------
void TutorialApplication::hostGame() {
    bool success = setupNetwork(true);
    CEGUI::Window *hostLabel = simulator->overlay->multiMenu->getChildRecursive("hostLabel");
    hostLabel->setText("Host Name: " + network.getIPstring());
    if(success) {
        CEGUI::Window *hostButton = simulator->overlay->multiMenu->getChildRecursive("HostButton");
        CEGUI::Window *joinButton = simulator->overlay->multiMenu->getChildRecursive("JoinButton");
        CEGUI::Window *joinBox = simulator->overlay->multiMenu->getChildRecursive("joinBox");
        joinBox->setDisabled(true);
        joinButton->setDisabled(true);
        hostButton->setDisabled(true);

        CEGUI::Window *p1joined = simulator->overlay->multiMenu->getChildRecursive("p1joined");
        p1joined->show();
        isMultiplayer = true;
    }
    else
        closeNetwork();

}
//---------------------------------------------------------------------------
void TutorialApplication::joinGame() {
    CEGUI::Window *joinBox = simulator->overlay->multiMenu->getChildRecursive("joinBox");
    hostName = joinBox->getText().c_str();
    bool success;
    success = setupNetwork(false);
    if(success) {
        CEGUI::Window *hostButton = simulator->overlay->multiMenu->getChildRecursive("HostButton");
        CEGUI::Window *joinButton = simulator->overlay->multiMenu->getChildRecursive("JoinButton");
        CEGUI::Window *p1joined = simulator->overlay->multiMenu->getChildRecursive("p1joined");
        CEGUI::Window *p2joined = simulator->overlay->multiMenu->getChildRecursive("p2joined");
        CEGUI::Window *startButton = simulator->overlay->multiMenu->getChildRecursive("StartButton");
        joinButton->setDisabled(true);
        hostButton->setDisabled(true);
        p1joined->show();
        p2joined->show();
        startButton->setText("Waiting for host");
        network.messageServer(PROTOCOL_TCP, "p2joined", 8);

        // Player positional/orientation/bullet pos coords
        positions.xPPos = 0.0f; positions.yPPos = 0.0f; positions.zPPos = -50.0f;
        positions.xPDir = 0.0f; positions.yPDir = 0.0f; positions.zPDir = 1.0f;
        positions.xPBPos = -100.0f; positions.yPBPos = -50.0f; positions.zPBPos = 0.0f;
        // // Enemy positional/orientation/ bullet pos coords;
        positions.xEPos = 0.0f; positions.yEPos = 0.0f; positions.zEPos = -10.0f;
        positions.xEDir = 0.0f; positions.yEDir = 0.0f; positions.zEDir = -1.0f;
        positions.xEBPos = 100.0f; positions.yEBPos = -50.0f; positions.zEBPos = 0.0f;

        Shooter* playerShooter = (Shooter*) simulator->getObject("PlayerShooter");
        playerShooter->setPosition(0.0f, 10.0f, -50.0f);

        mCamera->setPosition(playerShooter->getOgrePosition());
        mCamera->lookAt(0.0f, 10.0f, 0.0f);

        Shooter* enemyShooter = (Shooter*) simulator->getObject("CPUShooter");
        enemyShooter->setPosition(0.0f, 10.0f, 0.0f);

        isMultiplayer = true;
    }
    else
        closeNetwork();
}
//---------------------------------------------------------------------------
void TutorialApplication::startMulti() {
    network.messageClients(PROTOCOL_TCP, "Start", 5);
    multiPlayerStarted = true;

    //createObjects();
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
void TutorialApplication::updatePositions() {
    // Player positional/orientation/bullet pos coords
    Shooter* playerShooter = (Shooter*) simulator->getObject("PlayerShooter");
    if (playerShooter) {
        Ogre::Vector3 pPos = playerShooter->getOgrePosition();
        Ogre::Vector3 pDir = playerShooter->getOgreDirection() * Ogre::Vector3(0, 0, -1);
        positions.xPPos = pPos.x; positions.yPPos = pPos.y; positions.zPPos = pPos.z;
        positions.xPDir = pDir.x; positions.yPDir = pDir.y; positions.zPDir = pDir.z;
        positions.xPBPos = 0; positions.yPBPos = 0; positions.zPBPos = 0;
        positions.xPBVel = 0; positions.yPBVel = 0; positions.zPBVel = 0;
    }
    // Enemy positional/orientation/ bullet pos coords;
    EnemyShooter* enemyShooter = (EnemyShooter*) simulator->getObject("CPUShooter");
    if (enemyShooter) {
        Ogre::Vector3 ePos = enemyShooter->getOgrePosition();
        Ogre::Vector3 eDir = enemyShooter->getOgreDirection() * Ogre::Vector3(0, 0, -1);
        positions.xEPos = ePos.x; positions.yEPos = ePos.y; positions.zEPos = ePos.z;
        positions.xEDir = eDir.x; positions.yEDir = eDir.y; positions.zEDir = eDir.z;
        positions.xEBPos = 0; positions.yEBPos = 0; positions.zEBPos = 0;
        positions.xEBVel = 0; positions.yEBVel = 0; positions.zEBVel = 0;
    }
}
//---------------------------------------------------------------------------
std::string TutorialApplication::getPositionString() {
    //std::string s = "";
    Ogre::stringstream ss;

    ss<<positions.xPPos<<","<<positions.yPPos<<","<<positions.zPPos<<","<<
        positions.xPDir<<","<<positions.yPDir<<","<<positions.zPDir<<","<<
        positions.xPBPos<<","<<positions.yPBPos<<","<<positions.zPBPos;

    ss << ",z";
    //ss >> s;

    return ss.str();
}
//---------------------------------------------------------------------------
void TutorialApplication::decodePositionString(std::string positionString) {
    /*
    float xEPos, yEPos, zEPos;
    float xEDir, yEDir, zEDir;
    float xEBPos, yEBPos, zEBPos;
    */
    char* ps = (char*) malloc(positionString.length()+1);
    strcpy(ps, positionString.c_str()); // strcpy(destination, source)
    positions.xEPos = std::strtof(strtok(ps, ","), NULL);
    positions.yEPos = std::strtof(strtok(NULL, ","), NULL);
    positions.zEPos = std::strtof(strtok(NULL, ","), NULL);
    positions.xEDir = std::strtof(strtok(NULL, ","), NULL);
    positions.yEDir = std::strtof(strtok(NULL, ","), NULL);
    positions.zEDir = std::strtof(strtok(NULL, ","), NULL);
    positions.xEBPos = std::strtof(strtok(NULL, ","), NULL);
    positions.yEBPos = std::strtof(strtok(NULL, ","), NULL);
    positions.zEBPos = std::strtof(strtok(NULL, ","), NULL);
    free(ps);
}
//---------------------------------------------------------------------------
void TutorialApplication::checkMultiStart() {
    if (netStarted && !multiPlayerStarted) {
        if(network.pollForActivity(1)) {
            if(isHost) {
                std::istringstream ss(network.tcpClientData[0]->output);
                std::string s;
                ss >> s;
                if(!connectionMade) {
                    if(s.compare("p2joined") == 0) {
                        CEGUI::Window *p2joined = simulator->overlay->multiMenu->getChildRecursive("p2joined");
                        p2joined->show();
                        CEGUI::Window *startButton = simulator->overlay->multiMenu->getChildRecursive("StartButton");
                        startButton->setDisabled(false);
                        startButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::startMulti, this));
                        connectionMade = true;
                    }
                }
            } else {
                std::istringstream ss(network.tcpServerData.output);
                std::string s;
                ss >> s;
                if(s.compare("Start") == 0) {
                    multiPlayerStarted = true;
                    //createObjects();
                }
            }
        }
    }

    if(multiPlayerStarted && !gameStarted) {
        simulator->overlay->multiMenu->hide();
        simulator->pause(); // Unpause simulation
        gameStarted = true;
    }
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

    // aimanager->move(fe);
    // aimanager->shoot();

    // Update Ogre with Bullet's State
	if (this->simulator != NULL && !simulator->paused()) {
		//suppose you have 60 frames per second
        simulator->stepSimulation(fe.timeSinceLastFrame, 60.0f, 1.0f/60.0f);

    }

    // Update the mCamera
    Shooter* pShooter = (Shooter*) simulator->getObject("PlayerShooter");
    // mCamera->setPosition(pShooter->getOgrePosition() - mCamera->getDirection());
    updatePositions();
    mCamera->setPosition(pShooter->getOgrePosition() + Ogre::Vector3(0, 5, 5));

    if(isMultiplayer)
        checkMultiStart();

    if (gameStarted) {
        //if (isMultiplayer) {
        // printf("Creating the positions string\n");
        // char* positionString = (char*) malloc(sizeof(Positions) + 1);
        // printf("Copying the positions struct\n");
        // strcpy(reinterpret_cast<char*>(&positions), positionString);
        // printf("{%s}\n", positionString);
        // if (multiPlayerStarted) {
        //     if (isHost) {
        //         printf("Sending from host the positions\n");
        //         network.messageClients(PROTOCOL_TCP, positionString, sizeof(Positions));
        //     } else {
        //         printf("Sending to server the postions\n");
        //         network.messageServer(PROTOCOL_TCP, positionString, sizeof(Positions));
        //
        //         printf("Mission failed, we'll get wle;tjselkfm\n");
        //     }
        // }
        // printf("freeing the position string\n");
        // free(positionString);
        std::string positionString = getPositionString();
        if (multiPlayerStarted) {
            printf("Sending position string length: %d\nString: {%s}\n", positionString.length(), positionString.c_str());
            if (isHost) {
                network.messageClients(PROTOCOL_TCP, positionString.c_str(), positionString.length());
            } else {
                network.messageServer(PROTOCOL_TCP, positionString.c_str(), positionString.length());
            }
        }

        if (network.pollForActivity(1)) {
            if (isHost) {
                std::istringstream ss(network.tcpClientData[0]->output);
                std::string s = "";
                ss >> s;
                printf("Received string length [%d] : {%s}\n", s.length(), s.c_str());
                if(s.compare("pause") == 0) {
                    simulator->overlay->pauseGame();
                }

                // std::istringstream ssudp(network.udpClientData[0]->output);
                // s.clear();
                // ssudp >> s;
                else if (s.at(s.length()-1) == 'z') {
                    //Positions ePoses = *const_cast<Positions*>(reinterpret_cast<const Positions*>(s.c_str()));
                    // Update player position
                    decodePositionString(s);
                    btVector3 ePos = btVector3(positions.xEPos, positions.yEPos, positions.zEPos);
                    btVector3 eDir = btVector3(positions.xEDir, positions.yEDir, positions.zEDir);
                    btVector3 eBulletPos = btVector3(positions.xEBPos, positions.yEBPos, positions.zEBPos);

                    EnemyShooter* enemyShooter = (EnemyShooter*) simulator->getObject("CPUShooter");
                    enemyShooter->setNewPos(ePos);
                    enemyShooter->setNewDir(eDir);
                }
                //TODO Update our local positions
            } else {
                std::istringstream ss(network.tcpServerData.output);
                std::string s = "";
                ss >> s;
                printf("Received string length [%d] : {%s}\n", s.length(), s.c_str());
                if(s.compare("pause") == 0) {
                    simulator->overlay->pauseGame();
                } else if (s.at(s.length()-1) == 'z') {
                    //Positions ePoses = *const_cast<Positions*>(reinterpret_cast<const Positions*>(s.c_str()));
                    // Update player position
                    decodePositionString(s);
                    btVector3 ePos = btVector3(positions.xEPos, positions.yEPos, positions.zEPos);
                    btVector3 eDir = btVector3(positions.xEDir, positions.yEDir, positions.zEDir);
                    btVector3 eBulletPos = btVector3(positions.xEBPos, positions.yEBPos, positions.zEBPos);

                    EnemyShooter* enemyShooter = (EnemyShooter*) simulator->getObject("CPUShooter");
                    enemyShooter->setNewPos(ePos);
                    enemyShooter->setNewDir(eDir);
                }
                // std::istringstream ssudp(network.udpClientData[0]->output);
                // s.clear();
                // ssudp >> s;
                // if (s.length() != 0) {
                //     positions = *const_cast<Positions*>(reinterpret_cast<const Positions*>(s.c_str()));
                //     // Update player position, note: we are the enemy
                //     btVector3 myPos = btVector3(positions.xEPos, positions.yEPos, positions.zEPos);
                //     btVector3 myDir = btVector3(positions.xEDir, positions.yEDir, positions.zEDir);
                //     btVector3 myBulletPos = btVector3(positions.xEBPos, positions.yEBPos, positions.zEBPos);
                //
                //     // Enemy positional/orientation/ bullet pos coords;
                //     btVector3 ePos = btVector3(positions.xPPos, positions.yPPos, positions.zPPos);
                //     btVector3 eDir = btVector3(positions.xPDir, positions.yPDir, positions.zPDir);
                //     btVector3 eBulletPos = btVector3(positions.xPBPos, positions.yPBPos, positions.zPBPos);
                // }
                // //TODO
            }
        }
        //}
    }

    return ret;
}
//---------------------------------------------------------------------------
bool TutorialApplication::keyPressed(const OIS::KeyEvent& arg) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    if (arg.key == OIS::KC_ESCAPE) {
        quit();
    } else if (arg.key == OIS::KC_R) {
        //Reset the ball's position
        // Ball* ball = (Ball*) simulator->getObject("Ball");
        // ball->init();
    }
    else if(arg.key == OIS::KC_SPACE) {
        // Player* p = simulator->getPlayer("Player1");
        // if(!p->hasFired()) {
        //     GameObject* paddle = simulator->getObject("PlayerPaddle");
        //     Ogre::Vector3 location = (Ogre::Vector3) paddle->getPosition();
        //
        //     if(p->getNumShots() == 0) {
        //         Ball* laser = new Ball("plaser", mSceneMgr, simulator,
        //         Ogre::Vector3(location.x, location.y, location.z-20), 2.0f,
        //         "greenball", ballMass, ballRestitution, ballFriction, ballKinematic);
        //
        //         laser->setVelocity(btVector3(0, 0, -200));
        //     }
        //     else {
        //         Ball* laser = (Ball*) simulator->getObject("plaser");
        //         laser->setPosition(btVector3(location.x, location.y, location.z-20));
        //         laser->setVelocity(btVector3(0, 0, -200));
        //     }
        //     p->shot();
        //     simulator->soundSystem->playSound("laserSound");
        // }
        Shooter* player = (Shooter*) simulator->getObject("PlayerShooter");
        player->setVelocity(0,10,0);
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
    else if(arg.key == OIS::KC_M) {
        simulator->overlay->pauseGame();
        if(gameStarted && !isHost)
            network.messageServer(PROTOCOL_TCP, "pause", 7);
        else if(gameStarted && isHost)
            network.messageClients(PROTOCOL_TCP, "pause", 7);
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
    mCamera->yaw(Ogre::Degree(-arg.state.X.rel * cRotSpd));
    mCamera->pitch(Ogre::Degree(-arg.state.Y.rel * cRotSpd));

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

// Ogre::Vector2 getMouseScreenPosition() {
//     CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
//
//     return Ogre::Vector2(mousePos.d_x, mousePos.d_y);
// }
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

    //player->rotate(btQuaternion(0.0f, cDir.x, cDir.y, cDir.z));

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

    //printf("direction: (%f, %f, %f)\n", moveDir.x, moveDir.y, moveDir.z);


    // Ogre::Vector3 v = Ogre::Vector3(cDir.x, 0.0f, cDir.z);
    // float angle = v.dotProduct(Ogre::Vector3::UNIT_X);
    // //printf("Angle: %f\n", angle);

    // btQuaternion q = btQuaternion(btVector3(0.0f, 1.0f, 0.0f), angle);
    // player->rotate(q);

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
