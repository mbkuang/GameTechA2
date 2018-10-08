#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Ball.h>

Ball::Ball(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim)
    : GameObject(newName, scnMgr, sim) {

    Ogre::Entity* ball = sceneMgr->createEntity(name, "sphere.mesh");
    ball->setMaterialName("BallTexture");

    float radius = 2;

    ball->setCastShadows(true);
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(0,0,0));
    rootNode->attachObject(ball);
    rootNode->scale(radius * 0.01,radius * 0.01,radius * 0.01);
    rootNode->setPosition(0,0,-200);

    // Set the rigid Body
    transform.setIdentity();
    transform.setOrigin(btVector3(0, 0, -200));

    shape = new btSphereShape(btScalar(radius * .5));
    //TODO this->simulator->getCollisionShapes().push_back(shape);

    motionState = new OgreMotionState(transform, rootNode);

    mass = 1; //the mass is 1, because the ball is movable (dynamic)
    inertia = btVector3(0, 0, 0);

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState, shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(1);
    body->setUserPointer(rootNode);

    body->setLinearVelocity(btVector3(-35,50,-75));

    // Add to the physics simulator
    //this->simulator->getDynamicsWorld()->addRigidBody(body);
    this->simulator->addObject(this);
    //TODO this->simulator->trackRigidBodyWithName(body, "ballBody");
}

Ball::~Ball() {

}
