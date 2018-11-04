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

    // btVector3 pos = this->getPosition();
    // gun->setPosition(pos);
    // gun->rotate(this->getOgreDirection());
    // Ogre::Vector3 dir = this->getOgreDirection() * Ogre::Vector3(0.0f, 0.0f, -1.0f);
    // gun->setPosition(pos + btVector3(dir.x * 1.5f, 2.5f, dir.z * 1.5f));
    // lastTime += elapsedTime;
    // simulator->getDynamicsWorld()->contactTest(body, *cCallBack);

    // Ogre::String objName = this->getName();

    // if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
    //     && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
    //     //Handle the hit
    //     GameObject* contact = context->theObject;
    //     Ogre::String contactName = contact->getName();

    //     if(contactName.compare("PlayerLaser")) {
    //         Player* cpu = simulator->getPlayer("CPU");
    //         cpu->setHP(cpu->getHP()-1);
    //         simulator->overlay->updateScore();
    //     }

    //     lastTime = 0.0f;
    // }
    // context->hit = false;
}
