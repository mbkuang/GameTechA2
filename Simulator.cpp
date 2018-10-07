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
	dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
	//Add collision shapes to reuse among rigid bodies
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

    for (int i = 0; i < objList.size(); i++) {
    	btRigidBody* body = objList[i]->getBody();
    	Ogre::SceneNode* node = objList[i]->getNode();

    	btTransform worldTrans;
		body->getMotionState()->getWorldTransform(worldTrans);
		btQuaternion rot = worldTrans.getRotation();
		node->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
		btVector3 pos = worldTrans.getOrigin();
		node->setPosition(pos.x(), pos.y(), pos.z());
    }
}

// btDiscreteDynamicsWorld* Simulator::getDynamicsWorld() {
//     return dynamicsWorld;
// }

// std::vector<btCollisionShape*> Simulator::getCollisionShapes() {
//     return collisionShapes;
// }

// int Simulator::getCollisionObjectCount() {
//     return objMap.size();
// }

GameObject* Simulator::getObject(Ogre::String oName) {
    return objMap[oName];
}
