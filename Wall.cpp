#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <Wall.h>

Wall::Wall(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
    float xPosition, float yPosition, float zPosition,
    float xScale, float yScale, float zScale,
    Ogre::String material) :
    GameObject(newName, scnMgr, sim) {
    // Set the entity.
    geom = sceneMgr->createEntity(name, "cube.mesh");
    geom->setCastShadows(true);
    if (material != "")
        geom->setMaterialName(material);

    // Set the rootNode.
    rootNode = sceneMgr->getRootSceneNode()
        ->createChildSceneNode(name, Ogre::Vector3(xPosition, yPosition, zPosition));
    rootNode->attachObject(geom);
    rootNode->scale(xScale * 0.01, yScale * 0.01, zScale * 0.01);
    rootNode->setPosition(xPosition, yPosition, zPosition);

    // Set the rigid body.
    transform.setIdentity();
    transform.setOrigin(btVector3(xPosition, yPosition, zPosition));
    shape = new btBoxShape(btVector3(xScale * 0.5, yScale * 0.5, zScale * 0.5));
    motionState = new OgreMotionState(transform, rootNode);
    mass = 0;
    inertia = btVector3(0, 0, 0);
    shape->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo bRBInfo(
        mass, motionState, shape, inertia);
    body = new btRigidBody(bRBInfo);
    body->setRestitution(1);
    body->setFriction(0);
    body->setUserPointer(rootNode);
    this->simulator->addObject(this);
}
