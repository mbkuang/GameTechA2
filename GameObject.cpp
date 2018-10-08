#include "GameObject.h"

GameObject::GameObject(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim) {
	name = newName;
	sceneMgr = scnMgr;
	simulator = sim;
	rootNode = sceneMgr->getRootSceneNode();
	geom = NULL;
	shape = NULL;
	mass = 0.;
	body = NULL;
	inertia = btVector3(0, 0, 0);
	motionState = NULL;
}

GameObject::~GameObject() {
	//TODO :
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

void GameObject::setVelocity(float xVelocity, float yVelocity, float zVelocity) {
    if (body != NULL)
        body->setLinearVelocity(btVector3(xVelocity, yVelocity, zVelocity));
}

void GameObject::setVelocity(btVector3 newVelocity) {
    if (body != NULL)
        body->setLinearVelocity(newVelocity);
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
    }
}

void GameObject::move(Ogre::Real x, Ogre::Real y, Ogre::Real z) {
    rootNode->translate(rootNode->getLocalAxes(), x, y, z);
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
