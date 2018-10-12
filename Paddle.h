#ifndef __Paddle_h_
#define __Paddle_h_

#include <Ogre.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <GameObject.h>
#include <GameSettings.h>

class Paddle : public GameObject {
public:
    Paddle(Ogre::String newName, Ogre::SceneManager* scnMgr, Simulator* sim);
    ~Paddle();
    void move(Ogre::Real x, Ogre::Real y, Ogre::Real z);
};

#endif
