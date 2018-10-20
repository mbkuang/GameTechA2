#include "GameObject.h"

GameObject::GameObject(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim) {
    name = newName;
    sceneMgr = scnMgr;
    rootNode = NULL;
    geom = NULL;
    motionState = NULL;

    simulator = sim;
    shape = NULL;
    body = NULL;
    transform.setIdentity();
    inertia = btVector3(0.0f, 0.0f, 0.0f);

    mass = 0.0f;
    restitution = 0.0f;
    friction = 0.0f;
    kinematic = false;
    needsUpdates = false;

    context = NULL;
    cCallBack = NULL;
}

GameObject::~GameObject() {
	//TODO:
    simulator->removeObject(this);
    sceneMgr->destroyEntity(this->getName());
    this->rootNode = NULL;
}

btRigidBody* GameObject::getBody() {
	return this->body;
}

Ogre::String GameObject::getName() {
    return this->name;
}

OgreMotionState* GameObject::getOgreMotionState() {
    return this->motionState;
}

btVector3 GameObject::getPosition() {

    Ogre::SceneNode* gameNode = sceneMgr->getSceneNode(name);
    Ogre::Vector3 gamePosition = gameNode->getPosition();
    btVector3 position = btVector3(gamePosition.x, gamePosition.y, gamePosition.z);
    return position;
}

btVector3 GameObject::getVelocity() {
    if (body != NULL)
        return body->getLinearVelocity();
}

Ogre::Vector3 GameObject::getOgrePosition() {
    btVector3 pos = getPosition();
    return Ogre::Vector3(pos.getX(), pos.getY(), pos.getZ());
}

Ogre::Vector3 GameObject::getOgreVelocity() {
    btVector3 velocity = getVelocity();
    return Ogre::Vector3(velocity.getX(), velocity.getY(), velocity.getZ());
}

void GameObject::setVelocity(float xVelocity, float yVelocity, float zVelocity) {
    if (body != NULL) {
        body->setLinearVelocity(btVector3(xVelocity, yVelocity, zVelocity));
    }
}

void GameObject::setVelocity(btVector3 newVelocity) {
    if (body != NULL) {
        body->setLinearVelocity(newVelocity);
    }
}

void GameObject::setPosition(float xPosition, float yPosition, float zPosition) {
    if (body != NULL) {
        btTransform transformation;
        transformation.setOrigin(btVector3(xPosition, yPosition, zPosition));
        transformation.setRotation(body->getOrientation());
        body->setWorldTransform(transformation);
        motionState->setWorldTransform(transformation);
    }
}

void GameObject::setPosition(btVector3 newPosition) {
    if (body != NULL) {
        btTransform transformation;
        transformation.setOrigin(newPosition);
        transformation.setRotation(body->getOrientation());
        body->setWorldTransform(transformation);
        motionState->setWorldTransform(transformation);
        //motionState->updateTransform(transformation);
    }
}

void GameObject::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
    rootNode->translate(rootNode->getLocalAxes(), x, y, z);
    this->updateTransform();
}

void GameObject::updateTransform() {
    Ogre::Vector3 position = rootNode->getPosition();
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    Ogre::Quaternion quaternion = rootNode->getOrientation();
    transform.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z));
    if (motionState) {
        motionState->updateTransform(transform);
    }
}

// Add the game object to the simulator.
void GameObject::addToSimulator() {
    // Using motionstate is recommended, it provides interpolation capabilities,
    // and only synchronizes 'active' objects.
    updateTransform();
    // Rigid body is dynamic if and only if mass is non zero, otherwise static.
    if (mass != 0.0f) {
        shape->calculateLocalInertia(mass, inertia);
    }
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    rbInfo.m_restitution = restitution;
    rbInfo.m_friction = friction;
    body = new btRigidBody(rbInfo);
    body->setUserPointer(this);
    if (kinematic) {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);
    }
    context = new CollisionContext();
    cCallBack = new ContactSensorCallback(*body, *context);
    simulator->addObject(this);
}
