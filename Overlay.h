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
	//bool quit();
    bool countdown();
    void createMainMenu();
    bool onMainMenu();
    CEGUI::Window *sheet;
	CEGUI::Window *mainMenu;
	CEGUI::Window *settingsMenu;
	CEGUI::Window *playerScore;
	CEGUI::Window *cpuScore;
	bool done;

protected:
	CEGUI::OgreRenderer* mRenderer;
    CEGUI::Window *p1wins;
    int alarm;
	Simulator *simulator;
	bool onMMenu;

	bool singlePlayer();
	bool multiplayer();
	bool settings();
	bool quit();
	bool back();
};

#endif
