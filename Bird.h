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

    Shooter* target = NULL;
    Ogre::String leaderName = "Nobody";
    btVector3 formation;
    int state;
    enum States {
        FLY = 0,
        CHASE = 1,
        SCATTER = 2
    };
    btVector3 flyVector;
    int maxSpd = 125;
    int flySpd = 90;
    int minSpd = 70;
    int speed = 80;

    Ogre::ParticleEmitter* emitter = NULL;

    enum TIMES {
        SECOND = 60 * 20,
        FLYTIME = SECOND * 10,
        CHASETIME = SECOND * 3,
        CHASEFLYTIME = SECOND * 7,
        SCATTERTIME = SECOND * 2
    };
    int timer;

public:
    Bird(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator,
    	Ogre::Vector3 position, float radius, Ogre::String material,
    	float mass, float restitution, float friction, bool kinematic, Ogre::ParticleEmitter* particleEmit,
        int* numAttackers);
    ~Bird();
    void setTarget(Shooter* targe);
    void setLeader(Ogre::String lName);
    void setFormation(btVector3 form);
    btVector3 getDirection();
    void update(float elapsedTime);
    int getState();
    void chaseState();
    void flyState();
    void scatterState();

    void setVelocity(btVector3 vel);

    int* numAttack = NULL;
    bool attacking = false;
};

#endif
