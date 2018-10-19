#include "EnemyShooter.h"

EnemyShooter::EnemyShooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic)
        : Shooter(newName, scnMgr, sim, position, scale, material, mass,
        restitution, friction, kinematic) {
    // Set the entity.
    geom = sceneMgr->createEntity(name, "cube.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }
}

EnemyShooter::~EnemyShooter() {

}
