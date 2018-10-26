#include "Simulator.h"

Simulator::Simulator() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
			overlappingPairCache,
			solver,
			collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0, -98, 0.0));//0.0, 0.0));
	//Add collision shapes to reuse among rigid bodies

	soundSystem = new Sound();
	soundOn = false;

	overlay = new Overlay(this);
	isPaused = true;
}

void Simulator::addObject (GameObject* o) {
	objMap[o->getName()] = o;
    //TODO this crashes when we dont set object bodies
    objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
}

bool Simulator::removeObject(GameObject*  o) {
	objList.pop_back();
	objMap.erase(o->getName());
	Ogre::SceneNode* n = o->getNode();
	n = NULL;
	return true;
}

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
    dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	for (unsigned int i = 0; i < objList.size(); i++) {
		objList[i]->update(elapsedTime);

	}
}

btDiscreteDynamicsWorld* Simulator::getDynamicsWorld() {
    return dynamicsWorld;
}

GameObject* Simulator::getObject(Ogre::String oName) {
    return objMap[oName];
}

void Simulator::addPlayer(Player* p) {
	players[p->getName()] = p;
}

Player* Simulator::getPlayer(Ogre::String pName) {
    return players[pName];
}

/* Pauses or un-pauses the game */
void Simulator::pause() {
	isPaused = !isPaused;
}

/* Checks if the game is paused or not */
bool Simulator::paused() {
	return isPaused;
}
