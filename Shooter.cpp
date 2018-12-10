#include "Shooter.h"

Shooter::Shooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic)
    : GameObject(newName, scnMgr, sim) {
    // Set variables.
    this->position = position;
    this->scale = scale;
    this->material = material;
    this->mass = mass;
    this->restitution = restitution;
    this->friction = friction;
    this->kinematic = kinematic;
    this->orientation = btQuaternion(1,0,0,0);
    lastTime = 0.0f;

    // Set the entity.
    geom = sceneMgr->createEntity(name, "cube.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }

    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()->
        createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    rootNode->attachObject(geom);
    rootNode->scale(scale.x * 0.01f, scale.y * 0.01f, scale.z * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);

    // Set the rigid body.
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btBoxShape(btVector3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);

    gun = new Gun(nameGun("_Gun"), this->sceneMgr,
        this->position + Ogre::Vector3(0.0f, 2.5f, 1.5f), Ogre::Vector3(0.5f, 1.0f, 3.0f), this->material);

    addToSimulator();

    body->setAngularFactor(0.0f);

    fired = false;
    numShots = 0;

    jump = true;
    startPosition = position;
    fallenOff = false;
    door = false;
}

Shooter::~Shooter() {

}

Ogre::String Shooter::nameGun(Ogre::String gunName) {
    Ogre::String result = name;
    return result.append(gunName);
}

Ogre::Vector3 Shooter::getGunPosition() {
    btVector3 position = gun->getPosition();
    return Ogre::Vector3(position.getX(), position.getY(), position.getZ());
}

// Specific game object update routine.
void Shooter::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);

    btVector3 pos = this->getPosition();
    gun->setPosition(pos);
    gun->rotate(this->getOgreDirection());
    Ogre::Vector3 dir = this->getOgreDirection() * Ogre::Vector3(0.0f, 0.0f, -1.0f);
    gun->setPosition(pos + btVector3(dir.x * 1.5f, 2.5f, dir.z * 1.5f));

    if (context->hit) {
        jump = true;
        lastTime = 0.0f;

        GameObject* contact = context->theObject;
        Ogre::String contactName = contact->getName();

        if(contactName.compare("Door") == 0 && simulator->hasKC()) {
            door = true;
        }
    }
    context->hit = false;
}

// Has the player already shot at the oponent?
bool Shooter::hasFired() {
    return fired;
}

// Either the player just shot or the shot collided with something
void Shooter::shot() {
    // fired = !fired;
    // if(fired) {
    numShots++;
    //}
}

int Shooter::getNumShots() {
    return numShots;
}

bool Shooter::canJump() {
    return jump;
}

void Shooter::setJump(bool j) {
    jump = j;
}

void Shooter::setStartPos(Ogre::Vector3 pos) {
    startPosition = pos;
}

Ogre::Vector3 Shooter::getStartPos() {
    return startPosition;
}

bool Shooter::hasFallenOff() {
    return fallenOff;
}

void Shooter::setFallenOff(bool b) {
    fallenOff = b;
}

bool Shooter::reachedDoor() {
    return door;
}

void Shooter::setDoor(bool d) {
    door = d;
}
