#ifndef __Player_h_
#define __Player_h_

#ifndef __Player_h_DEFINED
    #define __Player_h_DEFINED
    class Player;
    #include "Simulator.h"
#endif

#include <Ogre.h>

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
