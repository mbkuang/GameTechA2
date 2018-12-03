#ifndef __Explosion_h_
#define __Explosion_h_

#include "GameObject.h"
#include "Laser.h"
#include <string>
#include <sstream>

class Explosion : public GameObject {
protected:
	Ogre::Vector3 position;
    float radius;
    Ogre::String material;
	float lastTime;
	Ogre::SceneNode* markerNode;
	Ogre::Entity* marker;
    bool availability;

public:
    Explosion(Ogre::String name, Ogre::SceneManager* sceneMgr, Simulator* simulator,
    	Ogre::Vector3 position, float radius, Ogre::String material,
    	float mass, float restitution, float friction, bool kinematic);
    ~Explosion();
    void update(float elapsedTime);
};

#endif
