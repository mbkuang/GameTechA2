#include <Paddle.h>

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
    transform.setIdentity();
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btBoxShape(btVector3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);

    inertia = btVector3(0, 0, 0);
    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState, shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(this->restitution);
    body->setFriction(this->friction);
    body->setUserPointer(rootNode);
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);

    addToSimulator();
}

Paddle::~Paddle() {

}

void Paddle::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
    rootNode->translate(rootNode->getLocalAxes(), x, y, z);
    Ogre::Vector3 pPosition = rootNode->getPosition();
    pPosition.x = std::min(std::max(pPosition.x, xMin), xMax);
    pPosition.y = std::min(std::max(pPosition.y, yMin), yMax);
    rootNode->setPosition(pPosition);
}

// Specific game object update routine.
void Paddle::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        //Handle the hit
        lastTime = 0.0f;
    }
    context->hit = false;
}
