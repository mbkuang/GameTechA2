#ifndef __Powerup_h_
#define __Powerup_h_

#include "GameObject.h"

class Powerup : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
	float lastTime;

    int type = 0;
    enum TYPES {
        HEALTH = 0,
        LIFE = 1,
        JUMP = 2,
        SHOTGUN = 3,
        CHECKPOINT = 4
    };

public:
    Powerup(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic, int newType);
    ~Powerup();
    int getType();
    void update(float elapsedTime);
};


#endif
