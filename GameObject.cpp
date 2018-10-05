#include "GameObject.h"

GameObject::GameObject(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator) {
	this->name = name;
	this->sceneMgr = sceneMgr;
	this->simulator = simulator;
	rootNode = sceneMgr->getRootSceneNode();
	geom = NULL;
	shape = NULL;
	mass = 0.;
	body = NULL;
	inertia = btVector3(0, 0, 0);
	motionState = NULL;
}

GameObject::~GameObject() {
	//TOD0:
}

btRigidBody* GameObject::getBody() {
	return this->body;
}
