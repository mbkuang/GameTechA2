#ifndef __Overlay_h_
#define __Overlay_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <sstream>
#include "Simulator.h"

class Simulator;

class Overlay {
public:
	Overlay(Simulator* simulator);
	~Overlay();
	void initCEGUI();
	bool updateScore();
	void createScoreboard();
    bool countdown();
    void createMainMenu();
    void pauseGame();
    void showWinMessage(int);
    void changeScoreboard();

    CEGUI::Window *sheet;
	CEGUI::Window *mainMenu;
	CEGUI::Window *settingsMenu;
	CEGUI::Window *pauseMenu;
	CEGUI::Window *musicMenu;
	CEGUI::Window *multiMenu;
	CEGUI::Window *newLevelMenu;
	CEGUI::Window *gameOverMenu;
	CEGUI::Window *deathMenu;
	CEGUI::Window *winMenu;
	CEGUI::Window *controlsMenu;
	CEGUI::Window *playerScore;
	CEGUI::Window *cpuScore;
	CEGUI::Window *levelInfo;
	bool done;

protected:
	CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window *p1wins;
    int alarm;
	Simulator *simulator;

	bool singlePlayer();
	// bool multiplayer();
	bool settings();
	bool quit();
	bool back();
	bool toMainMenu();
	bool showMusicMenu();
	void volumeDown();
	void volumeUp();
	void mute();
	void nextLevel();
	void leaveWinScreen();
	void playerLoses();
	void showControls();

	bool lastMenu;	//Keep track of what the last menu was before settings
	bool restarting;
};

#endif
