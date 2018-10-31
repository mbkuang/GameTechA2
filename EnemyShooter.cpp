#include "EnemyShooter.h"

EnemyShooter::EnemyShooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic)
        : Shooter(newName, scnMgr, sim, position, scale, material, mass,
        restitution, friction, kinematic) {
    // Set the entity.
    // geom = sceneMgr->createEntity(name, "cube.mesh");
    // geom->setCastShadows(true);
    // if (material != "") {
    //     geom->setMaterialName(material);
    // }

    // rootNode->attachObject(geom);
    this->newPos = btVector3(this->getPosition());
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
    this->rotate(btQuaternion(nDir.getX(), nDir.getY(), nDir.getZ()));
}

void EnemyShooter::update(float elapsedTime) {
    this->setVelocity(this->newPos - this->getPosition());
}
