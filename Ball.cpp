#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Ball.h>

Ball::Ball(Ogre::String newName, Simulator* sim, Ogre::SceneManager* scnMgr) {
    simulator = sim;

    name = newName;

    Ogre::Entity* ball = scnMgr->createEntity(name, "sphere.mesh");

    x = 0.0;
    y = 0.0;
    z = 0.0;

    ball->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(x,y,z));
    rootNode->attachObject(ball);
    rootNode->scale(0.1f,0.1f,0.1f);
    rootNode->setPosition(x,y,z);

    // Set the rigid Body
    transform.setIdentity();
    transform.setOrigin(btVector3(x, y, z));

    shape = new btSphereShape(btScalar(.5));
    //TODO this->simulator->getCollisionShapes().push_back(shape);

    motionState = new OgreMotionState(transform, rootNode);

    mass = .1; //the mass is 1, because the ball is movable (dynamic)
    inertia = btVector3(0, 0, 0);

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState->getMotionState(), shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(1);
    body->setUserPointer(rootNode);

    body->setLinearVelocity(btVector3(0,0,0));

    // Add to the physics simulator
    this->simulator->getDynamicsWorld()->addRigidBody(body);
    this->simulator->addObject(this);
    //TODO this->simulator->trackRigidBodyWithName(body, "ballBody");
}

Ball::~Ball() {

}
