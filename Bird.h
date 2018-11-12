#ifndef __Bird_h_
#define __Bird_h_

#include "GameObject.h"
#include "Shooter.h"
#include <string>
#include <sstream>

class Bird : public GameObject {
protected:
	Ogre::Vector3 position;
    float radius;
    Ogre::String material;
	float lastTime;
	Ogre::SceneNode* markerNode;
	Ogre::Entity* marker;
    bool availability;

    Shooter* target;
    int state;
    enum States {
        FLY = 0,
        CHASE = 1
    };
    btVector3 flyVector;
    int maxSpd = 100;
    int flySpd = 75;
    int minSpd = 20;
    int speed = 50;

public:
    Bird(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator,
    	Ogre::Vector3 position, float radius, Ogre::String material,
    	float mass, float restitution, float friction, bool kinematic);
    ~Bird();
    void setTarget(Shooter* targe);
    void update(float elapsedTime);
    void chaseState();
    void flyState();

    void setVelocity(btVector3 vel);
};

#endif
