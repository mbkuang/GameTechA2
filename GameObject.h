#ifndef __GameObject_h_
#define __GameObject_h_

#include "OgreMotionState.h"
#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
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
	btTransform tr;
	btVector3 inertia;
	OgreMotionState* motionState;
public:
	btRigidBody* getBody();
    Ogre::SceneNode* getNode() {return rootNode;}
};

#endif
