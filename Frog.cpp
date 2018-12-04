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
    geom = sceneMgr->createEntity(name, "sphere.mesh");
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
    Node* goal  = aiMgr->findNodeClosest(player_position);
    Node* next  = aiMgr->findNextNode(start, goal);

    // Ogre::Vector3 temp_pos = lerp(position, next->getPosition(), .5/1.5);
    // printf("Start pos: %f, %f, %f\n", position.x, position.y, position.z);
    // printf("Next pos: %f, %f, %f\n", temp_pos.x, temp_pos.y, temp_pos.z);
    // printf("Goal pos: %f, %f, %f\n", next->getPosition().x, next->getPosition().y, next->getPosition().z);

    /* Check if node froggo is planning to jump to is occupied */
    if(next->isOccupied()) {
        next = start;
    }

    /* Unoccupy a node and occupy node you're jumping to */
    start->occupy(false);
    next->occupy(true);

    startPosition = position;
    goalPosition = next->getPosition();

    // add an actual jump instead of a teleport
    // Ogre::Vector3 pos = next->getPosition();
    // position = pos;
    // rootNode->setPosition(pos.x, pos.y, pos.z);
    // updateTransform();

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
        lastContactTime = 0.0f;
    }

    context->hit = false;

    /* Make froggo jump */
    timeSinceLastJump += elapsedTime;
    if(timeSinceLastJump > 1.5) { // should probably adjust this value to make frogs faster/ slower
        position = goalPosition;
        rootNode->setPosition(goalPosition);
        updateTransform();
        jump();
        timeSinceLastJump = 0;
    } else {
        t = timeSinceLastJump / 1.5;
        Ogre::Vector3 temp_pos = lerp(goalPosition, startPosition, t);
        printf("Start pos: %f, %f, %f\n", startPosition.x, startPosition.y, startPosition.z);
        printf("Goal pos: %f, %f, %f\n", goalPosition.x, goalPosition.y, goalPosition.z);
        printf("updated position: %f, %f, %f\n", temp_pos.x, temp_pos.y, temp_pos.z);
        position = temp_pos;
        rootNode->setPosition(position);
        updateTransform();
    }
}

Ogre::Vector3 Frog::lerp(const Ogre::Vector3 A, const Ogre::Vector3 B, float t) {
    return A * t + B * (1.f - t);
}
