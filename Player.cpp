#include "Player.h"

Player::Player(Ogre::String name, Simulator* sim) {
	playerName = name;
	score = 0;
	sim->addPlayer(this);
}

void Player::incrementScore() {
	score++;
}

Ogre::String Player::getName() {
	return playerName;
}

int Player::getScore() {
	return score;
}
