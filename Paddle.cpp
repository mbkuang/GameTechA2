#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Paddle.h>

Paddle::Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim) : GameObject(newName, scnMgr, sim) {
    simulator = sim;

    name = newName;

    Ogre::Entity* paddle = scnMgr->createEntity(name, "cube.mesh");

    x = 0.0;
    y = 0.0;
    z = 0.0;

    paddle->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(x,y,z));
    rootNode->attachObject(paddle);
    rootNode->scale(.5f,.5f,.1f);
    rootNode->setPosition(x,y,z);

    //TODO Set the rigid Body
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(x, y, z));

    shape = new btBoxShape(btVector3(.5,.5,.1));

    motionState = new OgreMotionState(transform, rootNode);

    mass = 0; //the mass is 0, because the paddle is moved (controlled)
    inertia = btVector3(0, 0, 0);

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState->getMotionState(), shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(1);
    body->setUserPointer(rootNode);

    // Add to the physics simulator
    this->simulator->getDynamicsWorld()->addRigidBody(body);
    this->simulator->addObject(this);
}

Paddle::~Paddle() {

}
