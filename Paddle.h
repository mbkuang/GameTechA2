#ifndef __Paddle_h_
#define __Paddle_h_

#include "GameObject.h"
#include "GameSettings.h"

class Paddle : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
    float lastTime;
    //bool movable;
    //btQuaternion q;

public:
    bool movable;
    Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~Paddle();
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void set(btVector3 vel);
    void translate(btVector3 tr);
    void update(float elapsedTime);
};

#endif
