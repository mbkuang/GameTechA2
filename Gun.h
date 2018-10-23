#ifndef __Gun_h_
#define __Gun_h_

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include "GameSettings.h"

class Gun {
private:
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;
    
    Ogre::Vector3 position;
    Ogre::Vector3 scale;
    Ogre::String material;
    float lastTime;

public:
    Gun(Ogre::String newName, Ogre::SceneManager* scnMgr,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material);
    ~Gun();
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void setPosition(float newX, float newY, float newZ);
    void setPosition(btVector3 newPosition);
    void update(float elapsedTime);
};

#endif