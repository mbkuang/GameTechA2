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
	dynamicsWorld->setGravity(btVector3(0.0, 0.0, 0.0));
	//Add collision shapes to reuse among rigid bodies

	soundSystem = new Sound();
	soundOn = false;

	overlay = new Overlay(this);
}

void Simulator::addObject (GameObject* o) {
	objMap[o->getName()] = o;
    //TODO this crashes when we dont set object bodies
    objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
}

/*bool Simulator::removeObject(GameObject*  o) {

}*/

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
    dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	//for (int i = 0; i != objList.size(); i++) idList[i] = 0;
	dynamicsWorld->stepSimulation(elapsedTime, maxSubSteps, fixedTimestep);
	for (unsigned int i = 0; i < objList.size(); i++) {
		//if (objList[i]->doUpdates()) {
			objList[i]->update(elapsedTime);
		//}
	}
}

btDiscreteDynamicsWorld* Simulator::getDynamicsWorld() {
    return dynamicsWorld;
}

// std::vector<btCollisionShape*> Simulator::getCollisionShapes() {
//     return collisionShapes;
// }

// int Simulator::getCollisionObjectCount() {
//     return objMap.size();
// }

GameObject* Simulator::getObject(Ogre::String oName) {
    return objMap[oName];
}

void Simulator::addPlayer(Player* p) {
	players[p->getName()] = p;
}

Player* Simulator::getPlayer(Ogre::String pName) {
    return players[pName];
}
