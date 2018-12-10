#include "Player.h"

Player::Player(Ogre::String name, Simulator* sim) {
	playerName = name;
	score = 5;	// Score is lives
	hp = 5;
	sim->addPlayer(this);
	fired = false;
	numShots = 0;
	level = 0;
	killCount = 0;
}

// Actually decrements number of player's lives
void Player::incrementScore() {
	score--;
}

void Player::setScore(int newScore) {
    score = newScore;
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
	if(fired) {
		numShots++;
	}
}

int Player::getNumShots() {
	return numShots;
}

int Player::getLevel() {
	return level;
}

void Player::setLevel(int l) {
	level = l;
	killCount = 0;
}

int Player::getKC() {
	return killCount;
}

void Player::incrementKC() {
	killCount++;
}
