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

Paddle::~Paddle() {

}

void Paddle::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
    Ogre::Vector3 pPosition = rootNode->getPosition();
    float xNew = std::min(std::max(pPosition.x + x, xMin), xMax);
    float yNew = std::min(std::max(pPosition.y + y, yMin), yMax);
    float zNew = pPosition.z + z;
    this->setPosition(xNew, yNew, zNew);
    // float xt = std::min(std::max(x, xMin - pPosition.x), xMax - pPosition.x);
    // float yt = std::min(std::max(y, yMin - pPosition.y), yMax - pPosition.y);
    // rootNode->translate(rootNode->getLocalAxes(), xt, yt, z);
    // this->updateTransform();
}

// Specific game object update routine.
void Paddle::update(float elapsedTime) {
    // lastTime += elapsedTime;
    // simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    // if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
    //     && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
    //     //Handle the hit
    //     lastTime = 0.0f;
    // }
    // context->hit = false;
}
