#ifndef __GameObject_h_
#define __GameObject_h_

#ifndef __GameObject_h_DEFINED
    #define __GameObject_h_DEFINED
    class GameObject;
    #include "Simulator.h"
#endif


#include "OgreMotionState.h"
#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletDynamicsCommon.h>
class Simulator;

class GameObject {
protected:
	Ogre::String name;
	Ogre::SceneManager* sceneMgr;
	Simulator* simulator;
	Ogre::SceneNode* rootNode;
	Ogre::Entity* geom;
	btCollisionShape* shape;
	btScalar mass;
	btRigidBody* body;
	btTransform transform;
	btVector3 inertia;
	OgreMotionState* motionState;

public:
    GameObject(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim);
    ~GameObject();
	btRigidBody* getBody();
	Ogre::String getName();
    Ogre::SceneNode* getNode() {return rootNode;}
    btVector3 getPosition();
    btVector3 getVelocity();
    void setVelocity(float xVelocity, float yVelocity, float zVelocity);
    void setVelocity(btVector3 newVelocity);
    void setPosition(float xPosition, float yPosition, float zPosition);
    void setPosition(btVector3 newPosition);
};

#endif
