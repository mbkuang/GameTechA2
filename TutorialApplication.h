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
#include "Laser.h"
#include "Wall.h"
#include "Bird.h"
#include "GameSettings.h"
#include "Overlay.h"
#include "Shooter.h"

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

    void createLevel1();

    struct Positions {
        // Player positional/orientation/bullet pos coords
        float xPPos, yPPos, zPPos;
        float xPDir, yPDir, zPDir;
        float xPBPos, yPBPos, zPBPos;
        // Enemy positional/orientation/ bullet pos coords;
        float xEPos, yEPos, zEPos;
        float xEDir, yEDir, zEDir;
        float xEBPos, yEBPos, zEBPos;

        int pHealth, eHealth;
    };

    Positions positions;
private:
    bool processUnbufferedInput(const Ogre::FrameEvent& fe);
    void updatePositions();
    bool quit();
    void newGame();

    float old_x;
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
