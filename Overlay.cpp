#include "Overlay.h"

Overlay::Overlay(Simulator* sim) {
	simulator = sim;
    alarm = 0;
    done = false;
    lastMenu = true;
}

void Overlay::initCEGUI() {
	 /* Code taken/inspired by Ogre wiki */
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "CEGUIDemo/Sheet");
}

/* Load windows and buttons from layout files, and show main menu with all its members */
void Overlay::createMainMenu() {
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    mainMenu = wmgr.loadLayoutFromFile("mainMenu.layout");
    sheet->addChild(mainMenu);

    settingsMenu = wmgr.loadLayoutFromFile("settings.layout");
    settingsMenu->hide();
    sheet->addChild(settingsMenu);

    pauseMenu = wmgr.loadLayoutFromFile("pauseMenu.layout");
    pauseMenu->hide();
    sheet->addChild(pauseMenu);

    /* Main Menu Buttons */
    CEGUI::Window *singlePlayerButton = mainMenu->getChildRecursive("singlePlayerButton");
    singlePlayerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::singlePlayer, this));

    CEGUI::Window *multiplayerButton = mainMenu->getChildRecursive("multiplayerButton");
    multiplayerButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::multiplayer, this));

    CEGUI::Window *settingsButton = mainMenu->getChildRecursive("settingsButton");
    settingsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::settings, this));

    CEGUI::Window *quitButton = mainMenu->getChildRecursive("quitButton");
    quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::quit, this));

    /* Settings Buttons */
    CEGUI::Window *backSettings = settingsMenu->getChildRecursive("Setting4");
    backSettings->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::back, this));

    /* Pause Buttons */
    CEGUI::Window *resumeButton = pauseMenu->getChildRecursive("resumeButton");
    resumeButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::singlePlayer, this));

    CEGUI::Window *pauseSettings = pauseMenu->getChildRecursive("pauseSettings");
    pauseSettings->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::settings, this));

    CEGUI::Window *pauseBack = pauseMenu->getChildRecursive("pauseBack");
    pauseBack->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::back, this));

    CEGUI::Window *pauseQuit = pauseMenu->getChildRecursive("pauseQuit");
    pauseQuit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::quit, this));
}

/* Display the scoreboard */
void Overlay::createScoreboard() {
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	playerScore = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/StaticText");
    playerScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));

    cpuScore = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/StaticText");
    cpuScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    cpuScore->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0, 0)));

    updateScore();
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

/* Update the scoreboard when one of the players scores, and checks for wins */
void Overlay::updateScore() {
    Ogre::stringstream ss1;
    Ogre::stringstream ss2;

    Player* p1 = simulator->getPlayer("Player1");
    Player* cpu = simulator->getPlayer("CPU");

    int p1score = p1->getScore();
    int cpuscore = cpu->getScore();
    int p1hp = p1->getHP();
    int cpuhp = cpu->getHP();

    if(p1score == 3 || cpuhp == 0) {
        CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
        p1wins = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        p1wins->setSize(CEGUI::USize(CEGUI::UDim(.4, 0), CEGUI::UDim(.1, 0)));
        p1wins->setPosition(CEGUI::UVector2(CEGUI::UDim(.3, 0), CEGUI::UDim(0, 0)));
        p1wins->setText("[colour='FFFF0000']Player 1 wins!\n\nCPU gets tougher...");
        sheet->addChild(p1wins);
        p1->setScore(0);
        p1->setHP(5);
        cpu->setScore(0);
        cpu->setHP(5);
        alarm = 60*10*3;
    }
    else if(cpuscore == 3 || p1hp == 0) {
        CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
        p1wins = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        p1wins->setSize(CEGUI::USize(CEGUI::UDim(.4, 0), CEGUI::UDim(.1, 0)));
        p1wins->setPosition(CEGUI::UVector2(CEGUI::UDim(.3, 0), CEGUI::UDim(0, 0)));
        p1wins->setText("[colour='FFFF0000']CPU wins!\n\nYou must prove your worth!");
        sheet->addChild(p1wins);
        p1->setScore(0);
        p1->setHP(5);
        cpu->setScore(0);
        cpu->setHP(5);
        alarm = 60*10*3;
    }

    ss1 << "Player 1\nScore: "<<p1score<<"\nHP: "<<p1hp;
    ss2 << "CPU\nScore: "<<cpuscore <<"\nHP: "<<cpuhp;

    playerScore->setText("[colour='FFFF0000']"+ ss1.str());
    cpuScore->setText("[colour='FFFF0000']"+ ss2.str());
    sheet->addChild(playerScore);
    sheet->addChild(cpuScore);
    simulator->soundSystem->playSound("scoreSound");

}

/* Display the win/lose message for a certain amout of time  */
bool Overlay::countdown() {
    if (alarm > 0) {
        alarm --;
        if (alarm <= 0) {
            CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
            wmgr.destroyWindow(p1wins);
            updateScore();
            return true;
        }
    }
    return false;
}

/* Start Single player mode */
bool Overlay::singlePlayer() {
    mainMenu->hide();
    pauseMenu->hide();
    simulator->pause();
    return true;
}

/* Display the menu for multiplayer mode */
bool Overlay::multiplayer() {
    printf("Multiplayer Clicked!\n");
    return true;
}

/* Display the settings menu */
bool Overlay::settings() {
    mainMenu->hide();
    pauseMenu->hide();
    settingsMenu->show();
    return true;
}

/* Quit the application */
bool Overlay::quit() {
    done = true;
    return true;
}

/* Go back to the previous menu */
bool Overlay::back() {
    settingsMenu->hide();
    if(lastMenu || pauseMenu->isVisible()) {
        pauseMenu->hide();
        mainMenu->show();
    }
    else
        pauseMenu->show();
    return true;
}

/* Return to main menu */
bool Overlay::toMainMenu() {
    settingsMenu->hide();
    pauseMenu->hide();
    mainMenu->show();
}

/* Pause simulation and show the pause menu */
void Overlay::pauseGame() {
    lastMenu = false;
    simulator->pause();
    pauseMenu->show();
}
