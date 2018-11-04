#ifndef __EnemyShooter_h_
#define __EnemyShooter_h_

#include "Shooter.h"

class EnemyShooter : public Shooter {
protected:
    btVector3 newPos;
    float lastTime;
    Gun* gun;
public:
    EnemyShooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~EnemyShooter();
    void teleport();
    void setNewPos(btVector3 nPos);
    void setNewDir(btVector3 nDir);
    void update(float elapsedTime);
};

#endif
