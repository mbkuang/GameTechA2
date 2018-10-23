#ifndef __Overlay_h_
#define __Overlay_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <sstream>
#include "Simulator.h"

class Simulator;
class TutorialApplication;

class Overlay {
public:
	Overlay(Simulator* simulator);
	~Overlay();
	void initCEGUI();
	void updateScore();
	void createScoreboard();
	bool quit();
    bool countdown();
    void createMainMenu();
protected:
	CEGUI::OgreRenderer* mRenderer;
	CEGUI::Window *sheet;
	CEGUI::Window *mainMenu;
	CEGUI::Window *playerScore;
	CEGUI::Window *cpuScore;
    CEGUI::Window *p1wins;
    int alarm;
	Simulator *simulator;
};

#endif
