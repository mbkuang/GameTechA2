#include "EnemyShooter.h"

EnemyShooter::EnemyShooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic)
        : Shooter(newName, scnMgr, sim, position, scale, material, mass,
        restitution, friction, kinematic) {
    this->newPos = btVector3(this->getPosition());
    lastTime = 0.0f;
}

EnemyShooter::~EnemyShooter() {

}

void EnemyShooter::teleport() {
    this->setPosition(newPos);
}

void EnemyShooter::setNewPos(btVector3 nPos) {
    this->teleport();
    this->newPos = nPos;
    this->setVelocity(this->newPos - this->getPosition());
}

void EnemyShooter::setNewDir(btVector3 nDir) {
    this->rotate(btQuaternion(0, nDir.getY(), 0));
}

void EnemyShooter::update(float elapsedTime) {
    this->setVelocity(this->newPos - this->getPosition());
}
