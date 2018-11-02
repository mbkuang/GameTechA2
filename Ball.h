#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"
#include "Shooter.h"
#include <string>
#include <sstream>

class Ball : public GameObject {
protected:
	Ogre::Vector3 position;
    float radius;
    Ogre::String material;
	float lastTime;
	Ogre::SceneNode* markerNode;
	Ogre::Entity* marker;

public:
    Ball(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator,
    	Ogre::Vector3 position, float radius, Ogre::String material,
    	float mass, float restitution, float friction, bool kinematic);
    ~Ball();
    void init();
    Ogre::SceneNode* getMarkerNode();
    void update(float elapsedTime);
    void setVelocity(btVector3 vel);
    void setName(Ogre::String);
};

#endif
