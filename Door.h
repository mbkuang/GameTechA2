#ifndef __Door_h_
#define __Door_h_

#include "GameObject.h"
#include "Shooter.h"

class Door : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
	float lastTime;

public:
    Door(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~Door();

    bool tripped = false;
    bool active = true;

    void update(float elapsedTime);
};


#endif
