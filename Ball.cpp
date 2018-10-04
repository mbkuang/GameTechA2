#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Ball.h>

Ball::Ball(Simulator* sim, Ogre::SceneManager* scnMgr) {
    simulator = sim;
    Ogre::Entity* ball = scnMgr->createEntity("Sphere", "sphere.mesh");

    ball->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()
        ->createChildSceneNode("Ball", Ogre::Vector3(0,0,0));
    rootNode->attachObject(ball);
    rootNode->scale(0.1f,0.1f,0.1f);
    rootNode->setPosition(0,0,0);
    bRadius = 15.0f;
    bDirection = Ogre::Vector3(
        (rand()%100-50)/100.0f,
        (rand()%100-50)/100.0f,
        (rand()%100-50)/100.0f);
    bDirection.normalise();
    bSpeed = (rand()%200+50)/1.0f;

    //TODO Set the rigid Body
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(0, -50, 0));

    shape = new btSphereShape(btScalar(.5));
    //this->simulator->getCollisionShapes().push_back(shape);

    motionState = new OgreMotionState(transform, rootNode);

    mass = 1.; //the mass is 1, because the ball is movable (dynamic)
    inertia = btVector3(0, 0, 0);

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState, shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(1);
    body->setUserPointer(rootNode);

    // Add to the physics simulator
    this->simulator->getDynamicsWorld()->addRigidBody(body);
    this->simulator->addObject(this);
    //this->simulator->trackRigidBodyWithName(body, "ballBody")
}

Ogre::Vector3 Ball::getPosition() {
    return rootNode->getPosition();
}

Ogre::Real Ball::getRadius() {
    return bRadius;
}
