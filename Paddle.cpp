#include "Paddle.h"

Paddle::Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
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
    lastTime = 0.0f;
    movable = true;

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

    addToSimulator();

    //body->setSleepingThresholds(0.0f, 0.0f);
    body->setAngularFactor(0.0f);
    //body->setLinearFactor(btVector3(1.0f, 1.0f, 0.0f));
    body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}

Paddle::~Paddle() {

}

// void Paddle::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
//     // Update the Bullet object.
//     motionState->getWorldTransform(transform);
//     btVector3 position = transform.getOrigin();
//     btVector3 newPosition = btVector3(std::min(std::max(position.x() + x, xMin), xMax), std::min(std::max(position.y() + y, yMin), yMax), position.z() + z);
//     transform.setOrigin(newPosition);
//     body->setWorldTransform(transform);

//     // Update the Ogre object.
//     motionState->setWorldTransform(transform);
//     motionState->updateTransform(transform);
// }

void Paddle::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
    if (!movable) {
        return;
    }

    // Update the Bullet object.
    motionState->getWorldTransform(transform);
    btVector3 oldPosition = transform.getOrigin();
    btVector3 newPosition = btVector3(
        oldPosition.x() + x, 
        oldPosition.y() + y, 
        oldPosition.z() + z);
    transform.setOrigin(newPosition);
    body->setWorldTransform(transform);

    // Update the Ogre object.
    motionState->setWorldTransform(transform);
    motionState->updateTransform(transform);

    //printf("Movable: %d\n", movable);
}

void Paddle::set(btVector3 vel) {
    body->setLinearVelocity(vel);
}

void Paddle::translate(btVector3 tr) {
    body->translate(tr);
}

// Specific game object update routine.
void Paddle::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        //Handle the hit
        movable = false;
        //body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
        //body->clearForces();
        //body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
        //body->clearForces();
        //body->translate(btVector3(0.0f, 0.0f, 0.0f));
        lastTime = 0.0f;
    }
    context->hit = false;
    //movable = true;
}
