#include "Frog.h"

Frog::Frog(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, float radius, Ogre::String material, AIManager* aiMgr)
    : GameObject(newName, scnMgr, sim) {
    // Set variables.
    this->position = position;
    this->radius = radius;
    this->material = material;
    this->aiMgr = aiMgr;
    this->kinematic = true;
    this->mass = 1.0f; // ball mass
    this->restitution = .95f; // ball restitution
    this->friction =  0.0f; // ball friction
    this->timeSinceLastJump = 0.0f;
    this->lastContactTime = 0.0f;


    // Set the entity.
    geom = sceneMgr->createEntity(name, "cube.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }

    // Set the rootNode.
    this->rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    rootNode->attachObject(geom);
    rootNode->scale(radius * 0.01f, radius * 0.01f, radius * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);

    // Set the rigid body.
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btSphereShape(btScalar(radius));
    motionState = new OgreMotionState(transform, rootNode);

    addToSimulator();
    this->getBody()->setGravity(btVector3(0,0,0));

    // Edit spawn node
    Node* spawn = aiMgr->findNodeClosest(position);
    spawn->occupy(true);

    startPosition = position;
    goalPosition = position;
}

Frog::~Frog() {
    /* May not be neccesary? */
}

void Frog::jump() {
    /* Move frog to next node in it's path */
    Node* start = aiMgr->findNodeClosest(position);
    Ogre::Vector3 player_position = aiMgr->getPlayerPosition();
    Node* goal  = aiMgr->findNodeClosestPlayer(player_position, start);
    Node* next  = aiMgr->findNextNode(start, goal);

    /* Check if node froggo is planning to jump to is occupied */
    if(next->isOccupied()) {
        next = start;
    }

    /* Unoccupy a node and occupy node you're jumping to */
    start->occupy(false);
    next->occupy(true);

    /* Set jump parameters */
    startPosition = position;
    goalPosition = next->getPosition();
}

// Specific game object update routine.
void Frog::update(float elapsedTime) {

    /* Check if froggo hit something */
    lastContactTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    Ogre::String objName = this->getName();

    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
        && (lastContactTime > 0.5 || (context->lastBody != context->body && lastContactTime > 0.1))) {
        //Handle the hit
        lastContactTime = 0.0f;
        GameObject* contact = context->theObject;
        Ogre::String contactName = contact->getName();

        Player* p = simulator->getPlayer("Player1");
        Player* cpu = simulator->getPlayer("CPU");
        Shooter* ps = (Shooter*) simulator->getObject("PlayerShooter");

        simulator->soundSystem->playSound("deathSound");

        if (contactName.compare("PlayerShooter") == 0) {
            p->setHP(p->getHP()-1);
            simulator->overlay->updateScore();
            return;
        } else if (contactName.compare("EnemyShooter") == 0) {
            cpu->setHP(cpu->getHP()-1);
            simulator->overlay->updateScore();
            return;
        }
    }

    context->hit = false;

    /* Make froggo jump */
    timeSinceLastJump += elapsedTime;
    if(timeSinceLastJump > jumpPeriod) { // should probably adjust this value to make frogs faster/ slower
        position = goalPosition;
        rootNode->setPosition(goalPosition);
        updateTransform();
        jump();
        timeSinceLastJump = 0;
        hasShot = true;
    } else if (timeSinceLastJump > (jumpPeriod / 2) && startPosition != goalPosition) { // adjust value to adjust wait period
        /* Make frog travel to its next destination */
        float t = (timeSinceLastJump - (jumpPeriod / 2)) / (jumpPeriod / 2);

        /* Position between start and goal used to create jump arc */
        Ogre::Vector3 intermediatePosition(
            (startPosition.x + goalPosition.x) / 2,
            startPosition.y + 50, // jump height
            (startPosition.z + goalPosition.z) / 2
        );

        Ogre::Vector3 m1 = lerp(startPosition, intermediatePosition, t);
        Ogre::Vector3 m2 = lerp(intermediatePosition, goalPosition, t);

        Ogre::Vector3 temp_pos = lerp(m1, m2, t);
        position = temp_pos;
        rootNode->setPosition(position);
        updateTransform();
    } else {
        /* Interval between jumps, shooting? */
        if (hasShot) {
            if (timeSinceLastJump > 3 * jumpPeriod / 4) {
                Ogre::Vector3 position = this->getOgrePosition();
                Ogre::stringstream ss;
                ss << "EnemyLaser" << simulator->getObjectNumber("EnemyLaser") << "_" << this->name;

                Shooter* pShooter = (Shooter*) simulator->getObject("PlayerShooter");
                btVector3 lDir = btVector3(0,1,0);
                if (pShooter != NULL) {
                    lDir = (pShooter->getPosition() - this->getPosition()).normalized();
                }
                Laser* emitted = new Laser(ss.str(), sceneMgr, simulator,
                    position, 2.0f,
                    "greenball", ballMass, ballRestitution, ballFriction, ballKinematic);
                emitted->setPosition(this->getPosition() + lDir * 5);
                emitted->setVelocity(lDir * laserSpeed);
                hasShot = false;
            }
        }
    }
}

Ogre::Vector3 Frog::lerp(const Ogre::Vector3 A, const Ogre::Vector3 B, float t) {
    return B * t + A * (1.f - t);
}
