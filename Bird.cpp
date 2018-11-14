#include "Bird.h"

Bird::Bird(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
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
    availability = true;

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

    // Set the rigid body.
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btSphereShape(btScalar(radius * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);

    state = CHASE;
    flyVector = btVector3(0,0,0);

    addToSimulator();

    this->getBody()->setGravity(btVector3(0,0,0));
}

Bird::~Bird() {

}

void Bird::setTarget(Shooter* targe) {
    this->target = targe;
}

void Bird::setVelocity(btVector3 vel) {
    this->body->setLinearVelocity(vel);
}

// Specific game object update routine.
void Bird::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);

    Ogre::String objName = this->getName();
    if(objName.compare("EnemyBird") == 0)
        return;

    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1)) && state == CHASE) {
        //Handle the hit
        GameObject* contact = context->theObject;
        Ogre::String contactName = contact->getName();

        Player* p = simulator->getPlayer("Player1");
        Player* cpu = simulator->getPlayer("CPU");
        Shooter* ps = (Shooter*) simulator->getObject("PlayerShooter");

        simulator->soundSystem->playSound("deathSound");
        if (contactName.compare("PlayerShooter") == 0) {
            p->setHP(p->getHP()-1);
            simulator->overlay->updateScore();
            state = FLY;
            return;
        } else if (contactName.compare("EnemyShooter") == 0) {
            cpu->setHP(cpu->getHP()-1);
            simulator->overlay->updateScore();
            state = FLY;
            return;
        }

        lastTime = 0.0f;
    }
    context->hit = false;

    switch(state) {
        case FLY:
            //Fly around, trying to avoid being shot
            flyState();
            break;
        case CHASE:
            //Chase the player
            chaseState();
            break;
        default:
            state = FLY;
            break;
    }
}

void Bird::chaseState() {
    btVector3 vel = this->getVelocity();
    flyVector = target->getPosition() - this->getPosition();
    flyVector = flyVector.normalized() * speed;
    if (vel.angle(flyVector) > 15) {
        if (speed > minSpd) {speed --;}
        else {speed = minSpd;}
    }
    else if (speed < maxSpd) {speed ++;}
    else {speed = maxSpd;}
    this->setVelocity(this->getVelocity().lerp(flyVector, .001).normalized()*speed);
}

void Bird::flyState() {
    btVector3 vel = this->getVelocity();
    flyVector = vel.lerp(vel + btVector3(1,.01,-1), .1);
    flyVector = flyVector.normalized() * flySpd;
    this->setVelocity(this->getVelocity().lerp(flyVector, .05));
}
