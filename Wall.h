#ifndef __Wall_h_
#define __Wall_h_

#include "GameObject.h"

class Wall : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
	float lastTime;

public:
    Wall(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);  
    ~Wall();
    void update(float elapsedTime);
};


#endif