#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Wall.h>

Wall::Wall(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    float xPosition, float yPosition, float zPosition,
    float xScale, float yScale, float zScale) :
    GameObject(newName, scnMgr, sim) {

    Ogre::Entity* wall = sceneMgr->createEntity(name, "cube.mesh");
    wall->setMaterialName("WallTexture");

    wall->setCastShadows(true);
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(xPosition, yPosition, zPosition));
    rootNode->attachObject(wall);
    rootNode->scale(xScale * 0.01, yScale * 0.01, zScale * 0.01);
    rootNode->setPosition(xPosition, yPosition, zPosition);

    // Set the rigid body
    transform.setIdentity();
    transform.setOrigin(btVector3(xPosition, yPosition, zPosition));

    shape = new btBoxShape(btVector3(xScale * 0.5, yScale * 0.5, zScale * 0.5));
    //this->simulator->getCollisionShapes().push_back(shape);

    motionState = new OgreMotionState(transform, rootNode);

    mass = 0; //the mass is 0, because the wall is static
    inertia = btVector3(0, 0, 0);

    shape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState->getMotionState(), shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(1);
    body->setUserPointer(rootNode);
    //body->setWorldTransform(transform);

    // Add to the physics simulator
    //this->simulator->getDynamicsWorld()->addRigidBody(body);
    this->simulator->addObject(this);
    //this->simulator->trackRigidBodyWithName(body, "wallBody")
}
