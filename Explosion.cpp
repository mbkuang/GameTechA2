#include "Explosion.h"

Explosion::Explosion(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, float radius, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic)
    : GameObject(newName, scnMgr, sim) {
    // Set variables.
    this->position = position;
    this->radius = radius;
    this->material = material;
    this->mass = mass;
    this->restitution = restitution;
    this->friction = friction;
    this->kinematic = kinematic;
    lastTime = 0.0f;
    availability = true;

    // Set the entity.
    geom = sceneMgr->createEntity(name, "sphere.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }

    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    rootNode->attachObject(geom);
    rootNode->scale(radius * 0.01f, radius * 0.01f, radius * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);

    // Set the rigid body.
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btSphereShape(btScalar(radius * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);

    addToSimulator();
}

// Specific game object update routine.
void Laser::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);

    Ogre::String objName = this->getName();
}
