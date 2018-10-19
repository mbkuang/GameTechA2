#ifndef __EnemyShooter_h_
#define __EnemyShooter_h_

#include "Shooter.h"

class EnemyShooter : public Shooter {
public:
    EnemyShooter(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~EnemyShooter();
};

#endif
