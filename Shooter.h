#ifndef __Shooter_h_
#define __Shooter_h_

#include "GameObject.h"
#include "GameSettings.h"
#include "Gun.h"

class Shooter : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
    btQuaternion orientation;
    float lastTime;

    Gun* gun;

public:
    Shooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~Shooter();
    Ogre::String nameGun(Ogre::String gunName);
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void rotate(btQuaternion quat);
    void rotate(btVector3 axis, float angle);
    Ogre::Quaternion getOgreOrientation();
    btQuaternion getOrientation();
    void update(float elapsedTime);
};

#endif
