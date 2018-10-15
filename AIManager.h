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
    Paddle* ePaddle;
    Ball* ball;
    float movementSpeed;
    int level;
public:
    AIManager(Simulator* sim);
    AIManager(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Paddle* ePad, Ball* nBall);
    ~AIManager();
    void update(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Paddle* ePad, Ball* nBall);
    void advance();
    void move(const Ogre::FrameEvent& fe);
    void shoot();
};

#endif
