#ifndef __AIManager_h_
#define __AIManager_h_

#include <Ogre.h>

#ifndef __AIManager_h_DEFINED
    #define __AIManager_h_DEFINED
    class AIManager;
    #include "Paddle.h"
    #include "Ball.h"
    #include "GameSettings.h"
    #include "Simulator.h"
#endif

class Paddle;
class Ball;

class AIManager {
protected:
	Ogre::SceneManager* sceneMgr;
    Simulator* simulator;
    Paddle* paddle;
    Ball* ball;
    float movementSpeed = 60.0f;
public:
    AIManager(Simulator* sim);
    AIManager(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall);
    ~AIManager();
    void update(Ogre::SceneManager* scnMgr, Simulator* sim, Paddle* pad, Ball* nBall);
    void move(const Ogre::FrameEvent& fe);
};

#endif
