#include "Player.h"

Player::Player(Ogre::String name, Simulator* sim) {
	playerName = name;
	score = 0;
	hp = 5;
	sim->addPlayer(this);
	fired = false;
	numShots = 0;
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

int Player::getHP() {
	return hp;
}

int Player::setHP(int health) {
	hp = health;
}

// Has the player already shot at the oponent?
bool Player::hasFired() {
	return fired;
}

// Either the player just shot or the shot collided with something
void Player::shot() {
	fired = !fired;
	numShots++;
	printf("shot\n");
}

int Player::getNumShots() {
	return numShots;
}
