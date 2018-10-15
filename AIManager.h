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

public:
    AIManager(Simulator* sim);
    AIManager(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall);
    ~AIManager();
    void update(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall);
    void move(const Ogre::FrameEvent& fe);
    void shoot();
};

#endif
