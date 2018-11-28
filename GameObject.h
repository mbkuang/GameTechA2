#ifndef __GameObject_h_
#define __GameObject_h_

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include "OgreMotionState.h"
#include "Simulator.h"
#include "ContactSensorCallback.h"

class Simulator;

class GameObject {
protected:
	Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;
    OgreMotionState* motionState;

    Simulator* simulator;
    btCollisionShape* shape;
    btRigidBody* body;
    btTransform transform;
    btVector3 inertia;

    btScalar mass;
    btScalar restitution;
    btScalar friction;
    bool kinematic;
    bool needsUpdates;

    CollisionContext* context;
    ContactSensorCallback* cCallBack;

public:
    GameObject(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim);
    ~GameObject();
	btRigidBody* getBody();
	Ogre::String getName();
	OgreMotionState* getOgreMotionState();
    Ogre::SceneNode* getNode() {return rootNode;}
    btVector3 getPosition();
    btVector3 getVelocity();
    Ogre::Vector3 getOgrePosition();
    Ogre::Vector3 getOgreVelocity();
    void setVelocity(float xVelocity, float yVelocity, float zVelocity);
    void setVelocity(btVector3 newVelocity);
    void addVelocity(float xVelocity, float yVelocity, float zVelocity);
    void addVelocity(btVector3 newVelocity);
    void setPosition(float xPosition, float yPosition, float zPosition);
    void setPosition(btVector3 newPosition);
    btQuaternion getDirection();
    Ogre::Quaternion getOgreDirection();
    void rotate(float wDir, float xDir, float yDir, float zDir);
    void rotate(btQuaternion newDir);
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void updateTransform();
    void addToSimulator();
    virtual void update(float elapsedTime) {}
};

#endif
