#ifndef __Gun_h_
#define __Gun_h_

#include "GameObject.h"
#include "GameSettings.h"

class Gun : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
    float lastTime;

public:
    Gun(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~Gun();
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void update(float elapsedTime);
};

#endif
