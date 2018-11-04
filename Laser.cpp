#include "Laser.h"

Laser::Laser(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
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
    availability = false;

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

    addToSimulator();
}

Laser::~Laser() {

}

void Laser::setVelocity(btVector3 vel) {
    this->body->setLinearVelocity(vel);
}

bool Laser::isAvailable() {
    return availability;
}

// Specific game object update routine.
void Laser::update(float elapsedTime) {
    lastTime += elapsedTime;
    simulator->getDynamicsWorld()->contactTest(body, *cCallBack);

    if (context->hit && (context->velNorm > 2.0 || context->velNorm < -2.0)
        && (lastTime > 0.5 || (context->lastBody != context->body && lastTime > 0.1))) {
        //Handle the hit
        Ogre::String sw = "SouthWall";
        Ogre::String nw = "NorthWall";
        GameObject* contact = context->theObject;
        Ogre::String contactName = contact->getName();

        Ogre::String objName = this->getName();

        Player* p = simulator->getPlayer("Player1");
        Player* cpu = simulator->getPlayer("CPU");
        Shooter* ps = (Shooter*) simulator->getObject("PlayerShooter");
        if(objName.compare("b1") == 0 || objName.compare("b2") == 0 || objName.compare("b3") == 0) {
            p->shot();      //Update the firing status
            ps->shot();
            printf("zzz\n");
        }
        else if(objName.compare("cpulaser") == 0) {
            cpu->shot();    //Update the firing status
        }

        this->setPosition(100*ps->getNumShots(), 400, 0);  //Hide the projectile off screen
        //this->setVelocity(btVector3(0, 1, 0));
        this->inertia = btVector3(0.0f, 0.0f, 0.0f);
        availability = true;
        simulator->soundSystem->playSound("deathSound");
        this->context->reset(); //Reset the callback
        if(contactName.compare("PlayerPaddle") == 0) {
            p->setHP(p->getHP()-1);
            simulator->overlay->updateScore();
            return;
        }
        else if(contactName.compare("CPUPaddle") == 0) {
            cpu->setHP(cpu->getHP()-1);
            simulator->overlay->updateScore();
            return;
        }
        return;

        lastTime = 0.0f;
    }
    context->hit = false;
}
