#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Ball.h>

Ball::Ball(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim)
    : GameObject(newName, scnMgr, sim) {
    // Set the entity.
    geom = sceneMgr->createEntity(name, "sphere.mesh");
    geom->setCastShadows(true);
    geom->setMaterialName("BallTexture");

    // Set the rootNode.
    radius = 2.0f;
    x = 0.0f;
    y = 0.0f;
    z = -200.0f;
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(x, y, z));
    rootNode->attachObject(geom);
    rootNode->scale(radius * 0.01f,radius * 0.01f,radius * 0.01f);
    rootNode->setPosition(x, y, z);

    // Setup marker.
    marker = sceneMgr->createEntity("BallMarker", "cube.mesh");
    marker->setMaterialName("TransparentRed");
    marker->setCastShadows(false);
    markerNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode("BallMarker", Ogre::Vector3(x, y, z));
    markerNode->attachObject(marker);
    markerNode->scale(80.0f * 0.01f, 80.0f * 0.01f, 0.1f * 0.01f);
    markerNode->setPosition(x, y, z);

    // Set the rigid body.
    transform.setOrigin(btVector3(0, 0, -200));
    shape = new btSphereShape(btScalar(radius * .5));
    motionState = new OgreMotionState(transform, rootNode);
    mass = 1.0f;
    restitution = 1.01f;
    friction = 0.0f;

    addToSimulator();

    // Set the ball's velocity.
    float random = rand(); //(rand()%100-50);
    float xdir = sin(random) * 50;
    float ydir = (1 - sin(random)) * 50;
    body->setLinearVelocity(btVector3(xdir,ydir,-75));
}

Ball::~Ball() {
 // TODO:
}

Ogre::SceneNode* Ball::getMarkerNode() {
    return markerNode;
}
