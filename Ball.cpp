#include "Ball.h"

Ball::Ball(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, float radius, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic)
    : GameObject(newName, scnMgr, sim) {
    // Set variables.
    this->position = position;
    this->radius = radius;
    this->material = material;
    this->mass = mass;
    this->restitution = restitution;
    this->friction = friction;
    this->kinematic = kinematic;
    lastTime = 0.0f;

    // Set the entity.
    geom = sceneMgr->createEntity(name, "sphere.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }

    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    rootNode->attachObject(geom);
    rootNode->scale(radius * 0.01f, radius * 0.01f, radius * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);

    // Setup marker.
    marker = sceneMgr->createEntity("BallMarker", "cube.mesh");
    marker->setMaterialName("TransparentRed");
    marker->setCastShadows(false);
    markerNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode("BallMarker", Ogre::Vector3(position.x, position.y, position.z));
    markerNode->attachObject(marker);
    markerNode->scale(80.0f * 0.01f, 80.0f * 0.01f, 0.1f * 0.01f);
    markerNode->setPosition(position.x, position.y, position.z);

    // Set the rigid body.
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btSphereShape(btScalar(radius * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);

    addToSimulator();

    // Set the ball's velocity.
    float random = rand(); //(rand()%100-50);
    float xdir = sin(random) * 50.0f;
    float ydir = (1.0f - sin(random)) * 50.0f;
    body->setLinearVelocity(btVector3(xdir, ydir, -75.0f));
}

Ball::~Ball() {
 // TODO:
}

Ogre::SceneNode* Ball::getMarkerNode() {
    return markerNode;
}

// Specific game object update routine.
void Ball::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0) 
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        //Handle the hit
        printf("HIT\n");
        lastTime = 0.0f;
    }
    context->hit = false;
}
