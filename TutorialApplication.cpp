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

    initCEGUI();

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 25, 50);
    light->setDiffuseColour(1.0,1.0,1.0);
    lightNode->attachObject(light);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    Wall* flooring = new Wall("Flooring", mSceneMgr, simulator, 0, -49.5, 0, 100, 20, 100);
    Wall* ceiling = new Wall("Ceiling", mSceneMgr, simulator, 0, 49.5, 0, 100, 20, 100);
    Wall* westWall = new Wall("WestWall", mSceneMgr, simulator, -49.5, 0, 0, 20, 100, 100);
    Wall* eastWall = new Wall("EastWall", mSceneMgr, simulator, 49.5, 0, 0, 20, 100, 100);
    Wall* northWall = new Wall("NorthWall", mSceneMgr, simulator, 0, 0, -49.5, 100, 100, 20);
    Wall* southWall = new Wall("SouthWall", mSceneMgr, simulator, 0, 0, 49.5, 100, 100, 20);

    Ball* ball = new Ball("Ball", mSceneMgr, simulator);
    //ball->setPosition(0.0,5.0,0.0);
    //Ball* newball = new Ball("NewBall", mSceneMgr, simulator);
    //newball->setPosition(0.0,5.0,10.0);
    // Ball* newball2 = new Ball("NewBall2", mSceneMgr, simulator);
    // newball2->setPosition(0.0,50.0,25.0);
    Paddle* playerPaddle = new Paddle("PlayerPaddle", mSceneMgr, simulator);
    //playerPaddle->setPosition(0.0,0.0,-5.0);

    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

    sheet->addChild(quit);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    quit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&TutorialApplication::quit, this));

}
//---------------------------------------------------------------------------
bool TutorialApplication::quit() {
    mShutDown = true;
    return true;
}
//---------------------------------------------------------------------------
void TutorialApplication::initCEGUI() {
    /* Code taken/inspired by Ogre wiki */

    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

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

    // Update Ogre with Bullet's State
	if (this->simulator != NULL){
		//simulator->getDynamicsWorld()->stepSimulation(1.0f/60.0f); //suppose you have 60 frames per second
        simulator->stepSimulation(fe.timeSinceLastFrame);

		// for (int i = 0; i < this->simulator->getCollisionObjectCount(); i++) {
		// 	btCollisionObject* obj = this->simulator->getDynamicsWorld()->getCollisionObjectArray()[i];
		// 	btRigidBody* body = btRigidBody::upcast(obj);

		// 	if (body && body->getMotionState()){
		// 		btTransform trans;
		// 		body->getMotionState()->getWorldTransform(trans);

		// 		void *userPointer = body->getUserPointer();
		// 		if (userPointer) {
		// 			btQuaternion orientation = trans.getRotation();
		// 			Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
		// 			sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		// 			sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
		// 		}
		// 	}
		// }
	}

    return ret;
}
//---------------------------------------------------------------------------
bool TutorialApplication::keyPressed(const OIS::KeyEvent& arg) {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    if(arg.key == OIS::KC_ESCAPE) {
        mShutDown = true;
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
