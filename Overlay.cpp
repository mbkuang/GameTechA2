#include "Overlay.h"

Overlay::Overlay(Simulator* sim) {
	simulator = sim;
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
}

void Overlay::createScoreboard() {
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
	playerScore = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/StaticText");
    playerScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));

    cpuScore = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/StaticText");
    cpuScore->setSize(CEGUI::USize(CEGUI::UDim(0.1, 0), CEGUI::UDim(0.1, 0)));
    cpuScore->setPosition(CEGUI::UVector2(CEGUI::UDim(0.9, 0), CEGUI::UDim(0, 0)));

    updateScore();
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

void Overlay::updateScore() {
    Ogre::stringstream ss1;
    Ogre::stringstream ss2;

    Player* p1 = simulator->getPlayer("Player1");
    Player* cpu = simulator->getPlayer("CPU");

    int p1score = p1->getScore();
    int cpuscore = cpu->getScore();
    int p1hp = p1->getHP();
    int cpuhp = cpu->getHP();

    ss1 << "Player 1\nScore: "<<p1score<<"\nHP: "<<p1hp;
    ss2 << "Player 2\nScore: "<<cpuscore <<"\nHP: "<<cpuhp;

    playerScore->setText("[colour='FFFF0000']"+ ss1.str());
    cpuScore->setText("[colour='FFFF0000']"+ ss2.str());
    sheet->addChild(playerScore);
    sheet->addChild(cpuScore);
    simulator->soundSystem->playSound("scoreSound");

    if(p1score == 5 || cpuhp == 0) {
        CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
        CEGUI::Window *p1wins = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        p1wins->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        p1wins->setText("[colour='FFFF0000']Player 1 wins!\n\nPress 'Esc' to quit");
        //p1wins->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::quit(), this));
        sheet->addChild(p1wins);
        simulator->soundSystem->shutOffSound();
    }
    else if(cpuscore == 5 || p1hp == 0) {
        CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
        CEGUI::Window *p1wins = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
        p1wins->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
        p1wins->setText("[colour='FFFF0000']CPU wins!\n\nPress 'Esc' to quit");
        //p1wins->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Overlay::quit(), this));
        sheet->addChild(p1wins);
        simulator->soundSystem->shutOffSound();
    }
}
