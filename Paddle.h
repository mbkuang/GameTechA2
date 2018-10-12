#ifndef __Paddle_h_
#define __Paddle_h_

#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <GameObject.h>
#include <GameSettings.h>

class Paddle : public GameObject {
private:
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
    float lastTime;
public:
    Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material,
        float mass, float restitution, float friction, bool kinematic);
    ~Paddle();
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void update(float elapsedTime);
};

#endif
