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
	void updateScore();
	void createScoreboard();
    bool countdown();
    void createMainMenu();
    void pauseGame();

    CEGUI::Window *sheet;
	CEGUI::Window *mainMenu;
	CEGUI::Window *settingsMenu;
	CEGUI::Window *pauseMenu;
	CEGUI::Window *musicMenu;
	CEGUI::Window *multiMenu;
	CEGUI::Window *playerScore;
	CEGUI::Window *cpuScore;
	bool done;

protected:
	CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window *p1wins;
    int alarm;
	Simulator *simulator;

	bool singlePlayer();
	bool multiplayer();
	bool settings();
	bool quit();
	bool back();
	bool toMainMenu();
	bool showMusicMenu();

	bool lastMenu;	//Keep track of what the last menu was before settings
};

#endif
