#ifndef __AIManager_h_
#define __AIManager_h_

#include <Ogre.h>
#include "Simulator.h"
#include "Paddle.h"
#include "Ball.h"
#include "GameSettings.h"

class Simulator;
class Paddle;
class Ball;

class AIManager {
protected:
	Ogre::SceneManager* sceneMgr;
    Simulator* simulator;
    Paddle* paddle;
    Ball* ball;
    float movementSpeed;
    int level;
<<<<<<< HEAD
=======

>>>>>>> 4777af90bb8c329a6f0437c3a1bc0d1a879f42e8
public:
    AIManager(Simulator* sim);
    AIManager(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall);
    ~AIManager();
    void update(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall);
    void move(const Ogre::FrameEvent& fe);
    void shoot();
};

#endif
