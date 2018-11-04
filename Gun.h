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

public:
    Gun(Ogre::String newName, Ogre::SceneManager* scnMgr,
        Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::String material);
    ~Gun();
    Ogre::SceneNode* getRootNode();
    void setPosition(btVector3 newPosition);
    btVector3 getPosition();
    void rotate(Ogre::Quaternion quat);
};

#endif
