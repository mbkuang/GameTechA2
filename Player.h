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
    void setScore(int newScore);
	Ogre::String getName();
	int getScore();
	int getHP();
	int setHP(int);
	bool hasFired();
	void shot();
	int getNumShots();
	int getLevel();
	void setLevel(int);

protected:
	Ogre::String playerName;
	int score;
	int hp;
	Simulator* simulator;
	bool fired;
	int numShots;
	int level;
};

#endif
