#include "Spawner.h"

Spawner::Spawner(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic, float newType, float newRate, Ogre::ParticleSystem* particleSys) :
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
        Ogre::Vector3 pLocation = simulator->getObject("PlayerShooter")->getOgrePosition();
        Ogre::Vector3 eDir = pLocation - location;
        int avgVel = 50;
        GameObject* emitted;
        Ogre::ParticleEmitter* emitter = NULL;
        if (particleSystem != NULL) {
            emitter = particleSystem->addEmitter("Point");
            if (type != BIRD) {emitter->~ParticleEmitter();}
        }
        switch(type) {
            case BIRD:
                // Spawn a bird
                ss << "EnemyBird" << simulator->getObjectNumber();
                if (simulator->getObject(ss.str()) == NULL) {

                    emitted = new Bird(ss.str(), sceneMgr, simulator,
                        Ogre::Vector3(0, 10.0f, 300.0f), 2.0f,
                        "greenball", ballMass, ballRestitution, ballFriction, ballKinematic, emitter);
                    ((Bird*) emitted)->setTarget((Shooter*) simulator->getObject("PlayerShooter"));
                }
                break;
            case FROG:
                // Spawn a FROG
                break;
            case SHOOTER:
                // Spawn a shooter
                break;
            case LASER:
                // Spawn a laser (shoot at the player)
                ss << "EnemyLaser" << simulator->getObjectNumber();

                if (simulator->getObject(ss.str()) == NULL) {
                    //Ogre::ParticleEmitter* emitter = particleSystem->addEmitter("Point");
                    emitted = new Laser(ss.str(), sceneMgr, simulator,
                        Ogre::Vector3(0, 10.0f, 300.0f), 2.0f,
                        "greenball", ballMass, ballRestitution, ballFriction, ballKinematic);
                    emitted->setVelocity(simulator->getObject("PlayerShooter")->getPosition() - emitted->getPosition());
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
