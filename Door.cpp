#include "Door.h"

Door::Door(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic) :
    GameObject(newName, scnMgr, sim) {
    // Set variables.
    this->position = position;
    this->scale = scale;
    this->material = material;
    this->mass = mass;
    this->restitution = restitution;
    this->friction = friction;
    this->kinematic = kinematic;
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

    addToSimulator();
}

Door::~Door() {
    // TODO:
}

// Specific game object update routine.
void Door::update(float elapsedTime) {
    if (active) {
        lastTime += elapsedTime;
        simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
        if (context->hit) {
            // Handle the hit
            GameObject* contact = context->theObject;
            Ogre::String contactName = contact->getName();

            Player* p = simulator->getPlayer("Player1");
            Shooter* ps = (Shooter*) simulator->getObject("PlayerShooter");

            if (contactName.compare("PlayerShooter") == 0) {
                // Move to next room
                tripped = true;
                active = false;
                return;
            }

            lastTime = 0.0f;
        }
        context->hit = false;
    }
}