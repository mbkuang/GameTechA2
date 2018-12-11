#include "Spawner.h"

Spawner::Spawner(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic, int newType, float newRate, Ogre::ParticleSystem* particleSys, AIManager* aiManager, int* numAttackers) :
    GameObject(newName, scnMgr, sim) {
    // Set variables.
    this->position = position;
    this->scale = scale;
    this->material = material;
    this->mass = mass;
    this->restitution = restitution;
    this->friction = friction;
    this->kinematic = kinematic;
    lastTime = 0.0f;

    type = newType;
    rate = newRate;
    timer = newRate;

    aiMgr = aiManager;

    particleSystem = particleSys;

    // Set the entity.
    geom = sceneMgr->createEntity(name, "cube.mesh");
    geom->setCastShadows(true);
    if (material != "") {
        geom->setMaterialName(material);
    }

    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()->
        createChildSceneNode(name, Ogre::Vector3(position.x, position.y, position.z));
    rootNode->attachObject(geom);
    rootNode->scale(scale.x * 0.01f, scale.y * 0.01f, scale.z * 0.01f);
    rootNode->setPosition(position.x, position.y, position.z);

    // Set the rigid body.
    transform.setOrigin(btVector3(position.x, position.y, position.z));
    shape = new btBoxShape(btVector3(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f));
    motionState = new OgreMotionState(transform, rootNode);

    addToSimulator();

    numAttack = numAttackers;
}

Spawner::~Spawner() {
    // TODO:
}

// Specific game object update routine.
void Spawner::update(float elapsedTime) {

    timer -= elapsedTime;
    if (timer <= 0) {
        // Spawn whatever we're spawning
        timer = rate;
        Ogre::stringstream ss;
        Ogre::Vector3 location = this->getOgrePosition();
        Shooter* pShooter = (Shooter*) simulator->getObject("PlayerShooter");
        if (pShooter == NULL) {return;}

        Ogre::Vector3 pLocation = pShooter->getOgrePosition();
        Ogre::Vector3 eDir = pLocation - location;
        int avgVel = 50;
        GameObject* emitted;
        int numEmitted = 0;
        Ogre::Vector3 position = this->getOgrePosition();
        Ogre::ParticleEmitter* emitter = NULL;
        if (particleSystem != NULL) {
            if (type == BIRD) {emitter = particleSystem->addEmitter("Point");}
        }
        btVector3 lDir = btVector3(0,1,0);
        switch(type) {
            case BIRD:
                // Spawn a bird
                numEmitted = simulator->getObjectNumber("BirdEnemy");
                if (numEmitted < 10) {
                    ss << "BirdEnemy" << name << count;
                    count ++;
                    emitted = new Bird(ss.str(), sceneMgr, simulator,
                        position, 2.0f,
                        "BirdTexture", ballMass, ballRestitution, ballFriction, ballKinematic, emitter, numAttack);
                    ((Bird*) emitted)->setTarget(pShooter);
                }
                break;
            case FROG:
                // Spawn a FROG
                numEmitted = simulator->getObjectNumber("FrogEnemy");
                if (numEmitted < 5) {
                    ss << "FrogEnemy" << name << count;
                    count ++;
                    emitted = new Frog(ss.str(), sceneMgr, simulator,
                        position, 4.0f,
                        "BirdTexture", aiMgr, numAttack);
                }
                break;
            case SHOOTER:
                // Spawn a shooter
                break;
            case LASER:
                // Spawn a laser (shoot at the player)
                numEmitted = simulator->getObjectNumber("EnemyLaser");
                if (numEmitted < 10) {
                    ss << "EnemyLaser" << name << count;
                    count ++;
                    if (pShooter != NULL) {
                        lDir = (pShooter->getPosition() - this->getPosition()).normalized();
                    }
                    emitted = new Laser(ss.str(), sceneMgr, simulator,
                        position, 2.0f,
                        "greenball", ballMass, ballRestitution, ballFriction, ballKinematic);
                    emitted->setPosition(this->getPosition() + lDir * 10);
                    emitted->setVelocity(lDir * laserSpeed);
                }
                break;
            default:
                break;
        }
    }
    // lastTime += elapsedTime;
    // simulator->getDynamicsWorld()->contactTest(body, *cCallBack);
    // if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
    //     && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
    //     //Handle the hit
    //     lastTime = 0.0f;
    // }
    // context->hit = false;
}
