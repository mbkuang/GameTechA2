#include "Bird.h"

Bird::Bird(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    Ogre::Vector3 position, float radius, Ogre::String material,
    float mass, float restitution, float friction, bool kinematic, Ogre::ParticleEmitter* particleEmit,
    int* numAttackers)
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
    shape = new btSphereShape(btScalar(radius * 0.65f));
    motionState = new OgreMotionState(transform, rootNode);

    state = FLY;
    timer = SECOND*2;
    flyVector = btVector3(0,0,0);

    addToSimulator();

    this->getBody()->setGravity(btVector3(0,0,0));

    emitter = particleEmit;

    numAttack = numAttackers;
}

Bird::~Bird() {
    // if (emitter != NULL) {
    //     emitter->~ParticleEmitter();
    // }
    if (attacking) {
        (*numAttack) --;
        attacking = false;
    }
}

void Bird::setTarget(Shooter* targe) {
    this->target = targe;
}

void Bird::setLeader(Ogre::String lName) {
    this->leaderName = lName;
}

void Bird::setFormation(btVector3 form) {
    this->formation = form;
}

btVector3 Bird::getDirection() {
    return this->getVelocity().normalized();
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

    if (context->hit) {
        //Handle the hit
        GameObject* contact = context->theObject;
        Ogre::String contactName = contact->getName();

        Player* p = simulator->getPlayer("Player1");
        Player* cpu = simulator->getPlayer("CPU");
        Shooter* ps = (Shooter*) simulator->getObject("PlayerShooter");

        if (state == CHASE) {
            simulator->soundSystem->playSound("deathSound");
            if (contactName.compare("PlayerShooter") == 0) {
                p->setHP(p->getHP()-1);
                simulator->overlay->updateScore();
                state = FLY;//SCATTER;
                timer = FLYTIME;//SCATTERTIME;
                if (attacking) {(*numAttack) --; attacking = false;}
                return;
            } else if (contactName.compare("EnemyShooter") == 0) {
                cpu->setHP(cpu->getHP()-1);
                simulator->overlay->updateScore();
                state = FLY;
                timer = FLYTIME;
                return;
            }
        }

        // if(contactName.substr(0,11).compare("PlayerLaser") == 0 ||
        //     contactName.substr(0,10).compare("EnemyLaser") == 0) {
        //     simulator->removeObject(this);
        //     this->~GameObject();
        //     return;
        // }

        lastTime = 0.0f;
    }
    context->hit = false;

    Bird* leader = (Bird*) simulator->getObject(leaderName);
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

    // Get bird's velocity, what it wants its velocity to be, and where the player is looking
    btVector3 vel = this->getVelocity();
    flyVector = target->getPosition() - this->getPosition();
    btVector3 pLookDir = target->getLookDir().normalized() * -1;
    float vLength = flyVector.length();
    flyVector = flyVector.normalized();

    // If the player isn't looking at us, we can charge him, otherwise evade
    float angle = pLookDir.angle(flyVector);
    if (fabs(angle) < .1) {
        flyVector.setX(flyVector.getX() + sqrt(vLength)/50 - angle);
        flyVector.setY(flyVector.getY() + sqrt(vLength)/50 - angle);
        flyVector.setZ(flyVector.getZ() + sqrt(vLength)/50 - angle);
    }

    flyVector = flyVector.normalized() * speed;

    if (fabs(vel.angle(flyVector)) > .2) {
        if (speed > minSpd) {speed --;}
        else {speed = minSpd;}
        timer --;
        if (timer < 0) {
            timer = CHASEFLYTIME;
            state = FLY;
            attacking = false;
            (*numAttack) --;
        }
    }
    else if (speed < maxSpd) {speed ++;}
    else {speed = maxSpd;}

    this->setVelocity(this->getVelocity().lerp(flyVector, .005).normalized()*speed);
}

void Bird::flyState() {
    if (target == NULL) return;

    btVector3 vel = this->getVelocity();

    Bird* leader = (Bird*) simulator->getObject(leaderName);
    if (leader == NULL) {
        flyVector = vel.lerp(vel + btVector3(1,0,-1), .1);
        if (flyVector.getY() < 0) {flyVector.setY(flyVector.getY() + .1);}
        else if (flyVector.getY() > 30) {flyVector.setY(flyVector.getY() - .1);}
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
        if (lDist.length() > 10) {
            flyVector = lDir * speed;
        }
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
    // btVector3 vel = this->getVelocity();
    // // if (leader != NULL) {
    // //     flyVector = (leader->getPosition() + formation) - this->getPosition();
    // //     flyVector = flyVector.normalized() * speed;
    // // }
    // this->setVelocity(vel.lerp(flyVector, .005).normalized() * flySpd);

    btVector3 tDir = target->getPosition() - this->getPosition();
    Ogre::Vector3 otDir = Ogre::Vector3(tDir.getX(), tDir.getY(), tDir.getZ());
    Ogre::Vector3 tDirPerp = Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y) * otDir;
    flyVector = btVector3(tDirPerp.x, tDirPerp.y, tDirPerp.z);

    btVector3 vel = this->getVelocity();
    this->setVelocity(vel.lerp(flyVector, .005).normalized() * flySpd);

    timer --;
    if (timer < 0) {
        if (*numAttack < 5 && !attacking) {
            timer = CHASETIME;
            state = CHASE;
            (*numAttack) ++;
            attacking = true;
        } else {
            timer = rand() % CHASEFLYTIME;
        }
    }
}
