Simulator::Simulator() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
			overlappingPairCache,
			solver,
			collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0, -0.098, 0.0));
	//Add collision shapes to reuse among rigid bodies
}
void Simulator::addObject (GameObject* o) {
	objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
}