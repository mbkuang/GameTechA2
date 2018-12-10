#ifndef __Shooter_h_
#define __Shooter_h_

#include <OgreNode.h>
#include "GameObject.h"
#include "GameSettings.h"
#include "Gun.h"
#include "Player.h"
#include "Powerup.h"

class Shooter : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
    btQuaternion orientation;
    float lastTime;
    bool fired;
    int numShots;
    Gun* gun = NULL;
    Player* player = NULL;

    int weapon = 1;

    enum WEAPONTYPE {
        STANDARD = 0,
        SHOTGUN = 1,
        BIG = 2
    };

    float superJumpTimer = 0.0;

    bool jump;
    Ogre::Vector3 startPosition;
    bool fallenOff;
    bool door;

    Ogre::Vector3 lookDir;

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
    int getWeaponType();

    bool canJump();
    void setJump(bool);
    float getJumpTimer();

    void setStartPos(Ogre::Vector3);
    Ogre::Vector3 getStartPos();
    void setLookDir(Ogre::Vector3 newDir);
    Ogre::Vector3 getOgreLookDir();
    btVector3 getLookDir();
    bool hasFallenOff();
    void setFallenOff(bool);
    bool reachedDoor();
    void setDoor(bool);
};

#endif
