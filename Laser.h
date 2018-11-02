#ifndef __Laser_h_
#define __Laser_h_

#include "GameObject.h"
#include "Shooter.h"
#include <string>
#include <sstream>

class Laser : public GameObject {
protected:
	Ogre::Vector3 position;
    float radius;
    Ogre::String material;
	float lastTime;
	Ogre::SceneNode* markerNode;
	Ogre::Entity* marker;
    bool availability;

public:
    Laser(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator,
    	Ogre::Vector3 position, float radius, Ogre::String material,
    	float mass, float restitution, float friction, bool kinematic);
    ~Laser();
    void update(float elapsedTime);
    void setVelocity(btVector3 vel);
    bool isAvailable();
};

#endif
