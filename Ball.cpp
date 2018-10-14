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
    this->init();
}

Ball::~Ball() {
 // TODO:
}

void Ball::init() {
    this->setPosition(0, 0, -200);

    int c2random = 1000;

    int sign = rand() % 2 - 1;
    sign = sign < 0 ? -1 : 1;
    int arandom = sign * sqrt(rand() % c2random);

    sign = rand() % 2 - 1;
    sign = sign < 0 ? -1 : 1;
    int brandom = sign * sqrt(c2random - arandom*arandom);

    Ogre::Real x_dir = arandom;
    Ogre::Real y_dir = brandom;
    Ogre::Real z_dir = -75;

    this->body->setLinearVelocity(btVector3(x_dir, y_dir, z_dir));
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
        Ogre::String sw = "SouthWall";
        Ogre::String nw = "NorthWall";

        Ogre::String contactName = context->theObject->getName();
        if(contactName.compare("PlayerPaddle") == 0 
            || contactName.compare("CPUPaddle") == 0)
            simulator->soundSystem->playSound("paddleSound");
        else 
            simulator->soundSystem->playSound("wallSound");

        if(sw.compare(contactName) == 0) {
            simulator->getPlayer("CPU")->incrementScore();
            simulator->overlay->updateScore();
            this->init();
        }
        else if(nw.compare(contactName) == 0) {
            simulator->getPlayer("Player1")->incrementScore();
            simulator->overlay->updateScore();
            this->init();
        }
        lastTime = 0.0f;
    }
    context->hit = false;
}
