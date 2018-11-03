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
        this->position, Ogre::Vector3(1,1,5), this->material);

    addToSimulator();

    body->setAngularFactor(0.0f);

    fired = false;
    numShots = 0;
}

Shooter::~Shooter() {

}

Ogre::String Shooter::nameGun(Ogre::String gunName) {
    Ogre::String result = name;
    return result.append(gunName);
}

// Specific game object update routine.
void Shooter::update(float elapsedTime) {
    btVector3 pos = this->getPosition();

    gun->rotate(1,0,0,0);
    gun->setPosition(3,-2,-5);
    gun->getRootNode()->rotate(this->getOgreDirection(),
        Ogre::Node::TS_WORLD);
    
    pos += gun->getPosition();
    gun->setPosition(pos);

    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        //Handle the hit
        lastTime = 0.0f;
    }

    context->hit = false;
}

// Has the player already shot at the oponent?
bool Shooter::hasFired() {
    return fired;
}

// Either the player just shot or the shot collided with something
void Shooter::shot() {
    fired = !fired;
    if(fired) {
        numShots++;
    }
}

int Shooter::getNumShots() {
    return numShots;
}