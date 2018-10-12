#ifndef __Player_h_
#define __Player_h_

#include <Ogre.h>
#include "Simulator.h"

class Simulator;

class Player {
public:
	Player(Ogre::String name, Simulator*);
	~Player();

	void incrementScore();
	Ogre::String getName();
	int getScore();
	int getHP();
	int setHP(int);

protected:
	Ogre::String playerName;
	int score;
	int hp;
	Simulator* simulator;
};

#endif
