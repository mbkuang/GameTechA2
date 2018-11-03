#ifndef __Shooter_h_
#define __Shooter_h_

#include <OgreNode.h>
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
    bool fired;
    int numShots;
    Gun* gun;

public:
    Shooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~Shooter();
    Ogre::String nameGun(Ogre::String gunName);
    void update(float elapsedTime);
    bool hasFired();
    void shot();
    int getNumShots(); 
};

#endif
