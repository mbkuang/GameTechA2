#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Paddle.h>

Paddle::Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim)
    : GameObject(newName, scnMgr, sim) {
    Ogre::Entity* paddle = sceneMgr->createEntity(name, "cube.mesh");
    paddle->setMaterialName("PaddleTexture");

    float x = 0.0;
    float y = 0.0;
    float z = -11.0;

    float xScale = 12;
    float yScale = 10;
    float zScale = .5;

    paddle->setCastShadows(true);
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(x,y,z));
    rootNode->attachObject(paddle);
    rootNode->scale(xScale * .01, yScale * .01, zScale * .1 * .01);
    rootNode->setPosition(x,y,z);

    //TODO Set the rigid Body
    transform.setIdentity();
    transform.setOrigin(btVector3(x, y, z));

    shape = new btBoxShape(btVector3(xScale * .5,yScale * .5,zScale * .5));

    motionState = new OgreMotionState(transform, rootNode);

    mass = 0; //the mass is 0, because the paddle is moved (controlled)
    inertia = btVector3(0, 0, 0);

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState->getMotionState(), shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(1);
    body->setFriction(0);
    body->setUserPointer(rootNode);
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);

    // Add to the physics simulator
    //this->simulator->getDynamicsWorld()->addRigidBody(body);
    this->simulator->addObject(this);
}

Paddle::~Paddle() {

}
