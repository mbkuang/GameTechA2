/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include <btBulletDynamicsCommon.h>
#include "Simulator.h"
#include "Ball.h"
#include "Wall.h"
#include "Paddle.h"
#include "GameSettings.h"
#include "AIManager.h"
#include "Overlay.h"
#include "Shooter.h"
#include "EnemyShooter.h"
#include "NetManager.h"

//---------------------------------------------------------------------------

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    virtual void createScene(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& fe);

    virtual void createFrameListener(void);
    virtual bool keyPressed(const OIS::KeyEvent& arg);
    virtual bool keyReleased(const OIS::KeyEvent& arg);
    virtual bool mouseMoved(const OIS::MouseEvent& me);
    virtual bool mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID id);

    Simulator* simulator;
    AIManager* aimanager;

    /* Network Stuff */
    NetManager network;
    bool isHost;
    bool isMultiplayer;
    const char* hostName;
    int port_number;

    bool gameStarted;

    bool setupNetwork(bool);
    void closeNetwork();
    void hostGame();
    void joinGame();
    bool netStarted;
    bool connectionMade;
    void startMulti();
    bool multiPlayerStarted;

    void createObjects();

    struct Positions {
        // Player positional/orientation/bullet pos coords
        float xPPos, yPPos, zPPos;
        float xPDir, yPDir, zPDir;
        float xPBPos, yPBPos, zPBPos;
        // Enemy positional/orientation/ bullet pos coords;
        float xEPos, yEPos, zEPos;
        float xEDir, yEDir, zEDir;
        float xEBPos, yEBPos, zEBPos;
    };

    Positions positions;
private:
    bool processUnbufferedInput(const Ogre::FrameEvent& fe);
    bool quit();
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
