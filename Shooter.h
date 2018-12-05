#ifndef __Shooter_h_
#define __Shooter_h_

#include <OgreNode.h>
#include "GameObject.h"
#include "GameSettings.h"
#include "Gun.h"
#include "Player.h"

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
    bool jump;
    Ogre::Vector3 startPosition;
    bool fallenOff;

public:
    Shooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~Shooter();
    Ogre::String nameGun(Ogre::String gunName);
    Ogre::Vector3 getGunPosition();
    void update(float elapsedTime);
    bool hasFired();
    void shot();
    int getNumShots();
    bool canJump();
    void setJump(bool);
    void setStartPos(Ogre::Vector3);
    Ogre::Vector3 getStartPos();
    bool hasFallenOff();
    void setFallenOff(bool);
};

#endif
