#include "GameObject.h"

GameObject::GameObject() {

}

GameObject::~GameObject() {

}

btRigidBody* GameObject::getBody() {
	return this->body;
}
