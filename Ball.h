#ifndef __Ball_h_
#define __Ball_h_

#include "GameObject.h"

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
    Ogre::SceneNode* getMarkerNode();
    void update(float elapsedTime);
};

#endif
