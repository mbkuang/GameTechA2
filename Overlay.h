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

protected:
	CEGUI::OgreRenderer* mRenderer;
	CEGUI::Window *sheet;
	CEGUI::Window *playerScore;
	CEGUI::Window *cpuScore;
	Simulator *simulator;
};

#endif