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

    // gun = new Gun(nameGun("_Gun"), this->sceneMgr,
    //     this->position, Ogre::Vector3(1,1,5), this->material);

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

void Shooter::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
    Ogre::Vector3 pPosition = rootNode->getPosition();
    float xNew = pPosition.x + x;
    float yNew = pPosition.y + y;
    float zNew = pPosition.z + z;
    this->setPosition(xNew, yNew, zNew);
}

void Shooter::rotate(btQuaternion quat) {
    btTransform tr;
    motionState->getWorldTransform(tr);
    //this->orientation = quat;
    tr.setRotation(quat);
    motionState->setWorldTransform(tr);
    //motionState->updateTransform(transform);
}

void Shooter::rotate(btVector3 axis, float angle) {
    this->orientation.setRotation(axis, angle);
}

btQuaternion Shooter::getOrientation() {
    return this->orientation;
}

Ogre::Quaternion Shooter::getOgreOrientation() {
    btQuaternion quat = this->getOrientation();
    float w = quat.getW(), x = quat.getX(), y = -quat.getY(), z = quat.getZ();
    return Ogre::Quaternion(w, x, y, z);
}

// Specific game object update routine.
void Shooter::update(float elapsedTime) {
    // btVector3 pos = this->getPosition();

    // gun->rotate(1,0,0,0);
    // gun->setPosition(3,-2,-5);
    // gun->getRootNode()->rotate(this->getOgreOrientation(),
    //     Ogre::Node::TS_WORLD);
    //
    // pos += gun->getPosition();
    // gun->setPosition(pos);

    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        //Handle the hit
        GameObject* contact = context->theObject;
        Ogre::String contactName = contact->getName();

        // printf("COLLIDE\n");
        // if (contactName.compare("Flooring") == 0) {
        //     motionState->getWorldTransform(transform);
        //     btVector3 position = transform.getOrigin();
        //     position.setY(0.0f);
        //     transform.setOrigin(position);
        //     motionState->setWorldTransform(transform);
        //     motionState->updateTransform(transform);
        //     printf("RESET\n");
        // } else {
        //     this->body->clearForces();
        // }

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
    numShots++;
}

int Shooter::getNumShots() {
    return numShots;
}