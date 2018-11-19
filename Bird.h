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
    int maxSpd = 150;
    int flySpd = 110;
    int minSpd = 80;
    int speed = 80;

    enum TIMES {
        SECOND = 60 * 20,
        FLYTIME = SECOND * 20,
        CHASETIME = SECOND * 3,
        CHASEFLYTIME = SECOND * 10
    };
    int timer;

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
