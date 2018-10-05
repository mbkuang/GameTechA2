#include "GameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

btRigidBody* GameObject::getBody() {
	return this->body;
}

Ogre::String GameObject::getName() {
    return this->name;
}

void GameObject::setVelocity(float xVelocity, float yVelocity, float zVelocity) {
    if (body != NULL)
        body->setLinearVelocity(btVector3(xVelocity, yVelocity, zVelocity));
}

void GameObject::setPosition(float xPosition, float yPosition, float zPosition) {
    if (&transform != NULL)
        transform.setOrigin(btVector3(xPosition, yPosition, zPosition));
}
