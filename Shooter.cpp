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
    lastTime = 0.0f;
    printf("Initialized the player\n");

    // Set the entity.
    // geom = sceneMgr->createEntity(name, "cube.mesh");
    // geom->setCastShadows(true);
    // if (material != "") {
    //     geom->setMaterialName(material);
    // }

    // Set the rootNode.
    printf("Makin the root player\n");
    rootNode = sceneMgr->getRootSceneNode()->
        createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    //rootNode->attachObject(geom);
    rootNode->scale(scale.x * 0.01f, scale.y * 0.01f, scale.z * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);
    printf("Made the root player\n");

    // Set the rigid body.
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btBoxShape(btVector3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);
    printf("Made the rigid body player\n");

    printf("Makin the Gun\n");
    gun = new Gun(nameGun("_Gun"), this->sceneMgr, simulator,
        this->position, Ogre::Vector3(1,1,5), this->material,
        1, 0, 0, false);

    addToSimulator();
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

// Specific game object update routine.
void Shooter::update(float elapsedTime) {
    // lastTime += elapsedTime;
    // simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    // if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
    //     && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
    //     //Handle the hit
    //     lastTime = 0.0f;
    // }
    // context->hit = false;
}
