#include "Bird.h"

Bird::Bird(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, float radius, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic, Ogre::ParticleEmitter* particleEmit)
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
    timer = SECOND*2;
    flyVector = btVector3(0,0,0);

    addToSimulator();

    this->getBody()->setGravity(btVector3(0,0,0));

    emitter = particleEmit;
}

Bird::~Bird() {
    if (emitter != NULL) {
        emitter->~ParticleEmitter();
    }
}

void Bird::setTarget(Shooter* targe) {
    this->target = targe;
}

void Bird::setLeader(Bird* bird) {
    this->leader = bird;
}

void Bird::setFormation(btVector3 form) {
    this->formation = form;
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

    if (emitter != NULL) {
        emitter->setPosition(this->getOgrePosition());
        emitter->setDirection(this->getOgreVelocity());
    }

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
            state = SCATTER;
            timer = SCATTERTIME;
            return;
        } else if (contactName.compare("EnemyShooter") == 0) {
            cpu->setHP(cpu->getHP()-1);
            simulator->overlay->updateScore();
            state = FLY;
            timer = FLYTIME;
            return;
        }

        lastTime = 0.0f;
    }
    context->hit = false;

    if (leader != NULL && state != CHASE && state != SCATTER) {
        int lState = leader->getState();
        if (lState != CHASE) {
            state = lState;
            if (state == SCATTER) {
                timer = SCATTERTIME;
                state = SCATTER;
                if (target != NULL) {
                    btVector3 tDir = target->getPosition() - this->getPosition();
                    Ogre::Vector3 otDir = Ogre::Vector3(tDir.getX(), tDir.getY(), tDir.getZ());
                    Ogre::Vector3 tDirPerp = Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Z) * otDir;
                    flyVector = btVector3(tDirPerp.x, tDirPerp.y, tDirPerp.z);
                }
            }
        }
    }

    switch(state) {
        case FLY:
            //Fly around, trying to avoid being shot
            flyState();
            break;
        case CHASE:
            //Chase the player
            chaseState();
            break;
        case SCATTER:
            //Scatter!
            scatterState();
            break;
        default:
            state = FLY;
            break;
    }
}

int Bird::getState() {
    return state;
}

void Bird::chaseState() {
    if (target == NULL) return;

    btVector3 vel = this->getVelocity();
    flyVector = target->getPosition() - this->getPosition();
    flyVector = flyVector.normalized() * speed;
    if (fabs(vel.angle(flyVector)) > .26) {
        if (speed > minSpd) {speed --;}
        else {speed = minSpd;}
        timer --;
        if (timer < 0) {
            timer = CHASEFLYTIME;
            state = FLY;
        }
    }
    else if (speed < maxSpd) {speed ++;}
    else {speed = maxSpd;}
    this->setVelocity(this->getVelocity().lerp(flyVector, .001).normalized()*speed);
}

void Bird::flyState() {
    if (target == NULL) return;

    btVector3 vel = this->getVelocity();
    if (leader == NULL) {
        flyVector = vel.lerp(vel + btVector3(1,0,-1), .1);
        if (flyVector.getY() < 0) {flyVector.setY(flyVector.getY() + .1);}
        flyVector = flyVector.normalized() * flySpd;
        this->setVelocity(vel.lerp(flyVector, .05));

        timer --;
        if (timer < 0) {
            timer = SCATTERTIME;
            state = SCATTER;
            if (target != NULL) {
                btVector3 tDir = target->getPosition() - this->getPosition();
                Ogre::Vector3 otDir = Ogre::Vector3(tDir.getX(), tDir.getY(), tDir.getZ());
                Ogre::Vector3 tDirPerp = Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Z) * otDir;
                flyVector = btVector3(tDirPerp.x, tDirPerp.y, tDirPerp.z);
            }
        }
    } else {
        btVector3 lDist = (leader->getPosition() + formation) - this->getPosition();
        btVector3 lDir = lDist.normalized();
        flyVector = lDir * speed;
        if (fabs(vel.angle(flyVector)) > .26) {
            if (speed > minSpd) {speed --;}
            else {speed = minSpd;}
        }
        else if (speed < maxSpd) {speed ++;}
        else {speed = maxSpd;}
        this->setVelocity(this->getVelocity().lerp(flyVector, .005).normalized()*speed);
    }
}

void Bird::scatterState() {
    btVector3 vel = this->getVelocity();
    // if (leader != NULL) {
    //     flyVector = (leader->getPosition() + formation) - this->getPosition();
    //     flyVector = flyVector.normalized() * speed;
    // }
    this->setVelocity(vel.lerp(flyVector, .005).normalized() * flySpd);

    timer --;
    if (timer < 0) {
        timer = CHASETIME;
        state = CHASE;
    }
}
