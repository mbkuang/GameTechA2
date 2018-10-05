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
	dynamicsWorld->setGravity(btVector3(0.0, -.98, 0.0));//-0.098, 0.0));
	//Add collision shapes to reuse among rigid bodies
}

void Simulator::addObject (GameObject* o) {
	objMap[o->getName()] = o;
    //TODO this crashes when we dont set object bodies
	dynamicsWorld->addRigidBody(o->getBody());
}

/*bool Simulator::removeObject(GameObject*  o) {

}*/

void Simulator::stepSimulation(const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
    getDynamicsWorld()->stepSimulation(elapsedTime,maxSubSteps,fixedTimestep);//1.0f/60.0f,);
}

btDiscreteDynamicsWorld* Simulator::getDynamicsWorld() {
    return dynamicsWorld;
}

std::vector<btCollisionShape*> Simulator::getCollisionShapes() {
    return collisionShapes;
}

int Simulator::getCollisionObjectCount() {
    return objMap.size();
}

GameObject* Simulator::getObject(Ogre::String oName) {
    return objMap[oName];
}
