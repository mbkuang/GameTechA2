#ifndef __Spawner_h_
#define __Spawner_h_

#include "GameObject.h"
#include "Bird.h"
#include "Frog.h"
#include "Laser.h"
#include "AIManager.h"
#include <OgreParticleSystem.h>

class Spawner : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
	float lastTime;

    int type;
    float rate;
    float timer;
    int count = 0;

    Ogre::ParticleSystem* particleSystem = NULL;
    AIManager* aiMgr;

    enum Types {
        BIRD = 0,
        FROG = 1,
        SHOOTER = 2,
        LASER = 3
    };

public:
    Spawner(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic, int newType,
        float newRate, Ogre::ParticleSystem* particleSys, AIManager* aiManager);
    ~Spawner();
    void update(float elapsedTime);
};


#endif
